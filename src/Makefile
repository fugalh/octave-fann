SRC = fann_create.cc fann_randomize_weights.cc fann_set_parameters.cc fann_get_parameters.cc fann_run.cc __fann_train_on_file.cc fann_print_connections.cc fann_save.cc fann_size.cc fann_read_data.cc fann_get_mse.cc fann_reset_mse.cc __fann_test_data_from_file.cc __fann_init_weights_on_file.cc
M = fann_merge_data.m fann_save_train.m fann_test.m fann_reinit_weights.m fann_shuffle_data.m fann_train.m
OCT = $(patsubst %.cc, %.oct, $(SRC))
OBJ = $(patsubst %.cc, %.o, $(SRC))

.phony: all
all: $(OCT)

%.oct: %.cc oct_fann.h
	mkoctfile -lfann $< 

.phony: clean
clean:
	rm -f $(OCT) $(OBJ)

install: all
	cp $(M) `octave-config --print LOCALFCNFILEDIR`
	cp $(OCT) `octave-config --print LOCALAPIOCTFILEDIR`

uninstall:
	cd `octave-config --print LOCALFCNFILEDIR`; rm -f $(M)
	cd `octave-config --print LOCALAPIOCTFILEDIR`; rm -f $(OCT)
