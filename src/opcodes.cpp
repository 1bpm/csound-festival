#include <plugin.h>
#include <festival/festival.h>
#include <sstream>

struct ftts : csnd::Plugin<1, 2> {
	static constexpr char const *otypes = "i";
	static constexpr char const *itypes = "Si";
	EST_Wave wave;
	FUNC *table;

	int genft(int size) {
		EVTBLK *evt;
		MYFLT *pf;

		evt = (EVTBLK*) csound->malloc(sizeof(EVTBLK));
		evt->opcod = "f";
		evt->strarg = NULL;
		evt->pcnt = 5;
		pf = &evt->p[0];
		pf[0] = FL(0);
		pf[1] = FL(0);
		pf[2] = evt->p2orig = FL(0);
		pf[3] = evt->p3orig = -size;
		pf[4] = FL(2); // gen numbe
		pf[5] = FL(0);
		int n = csound->get_csound()->hfgens(csound->get_csound(), &table, evt, 1);
		if (UNLIKELY(n != 0)) {
			return NOTOK;
		}
		csound->free(evt);
		return OK;
	}

	int init() {
		// create mutex for realtime run; check csound.c like line 3685
		STRINGDAT &word = inargs.str_data(0);
		int heap_size = 210000;
		int load_init_files = 1;
		festival_initialize(load_init_files, heap_size);
		std::ostringstream pitch;
		pitch << "(set! duffint_params '((start "
			<< ((int) inargs[1]) << ") end ("
			<< ((int) inargs[1]) << ")))";
		festival_eval_command(pitch.str().c_str());

		festival_eval_command("(Parameter.set 'Int_Method 'DuffInt)");
		festival_eval_command("(Parameter.set 'Int_Target_Method Int_Targets_Default)");
		festival_eval_command("(Parameter.set 'Audio_Required_Rate 44100)");
		festival_text_to_wave(word.data, wave);
		festival_wait_for_spooler();
	
		int adjusted_size = (csound->sr() / wave.sample_rate()) * wave.length();
		if ((genft(adjusted_size)) != OK) {
			return csound->init_error("Cannot create ftable");
		}

		table->soundend = adjusted_size;
		table->nchanls = 1;
		table->flenfrms = adjusted_size;
		table->gen01args.sample_rate = csound->sr();
		table->cpscvt = 0;
		table->cvtbas = LOFACT; // * csound->sr() * csound->get_csound()->onedsr;
		table->loopmode1 = 0;
		table->loopmode2 = 0;
		table->begin1 = 0;
		table->end1 = adjusted_size;
		table->begin2 = 0;
		table->end2 = adjusted_size;
		
		read_in();
		outargs[0] = table->fno;
		return OK;
	}

	int read_in() {
		MYFLT lastsample = 0;
		int read_pos = 0;
		int kcount = 0;
		int write_pos = 0;
		int srdiv = csound->sr() / wave.sample_rate();
		
		while (read_pos < wave.length() && write_pos < table->soundend) {
			if (kcount == 0) {
				lastsample = (MYFLT) wave.a(read_pos++, 0)*0.00001;
			}

			table->ftable[write_pos] = lastsample;

			write_pos++;

			if (++kcount > srdiv) {
				kcount = 0;
			}
		}
		return OK;
	}
};

#include <modload.h>
void csnd::on_load(csnd::Csound *csound) {
	csnd::plugin<ftts>(csound, "ftts", csnd::thread::i);
}
