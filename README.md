# DSP_HW1

* This work implements the discrete Hidden Markov Model (HMM), which will be trained and tested by two datasets.
* Training: Baum-Welch algorithm, a generalized expectation-maximization (EM) algorithm.
* Testing: Viterbi algorithm.

### Usage
* Training
./train iteration model_init.txt seq_model_(01~05).txt model_(01~05).txt

iteration: assigned iteration #

* Testing
./test modellist.txt testing_data.txt result.txt

* For more details, please refer to the file dsp_hw1.pdf.
