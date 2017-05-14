if [ $# -ne 1 ]; then
	exit 1
fi
finaliter=$1;

if [ -f acc.txt ]; then
	rm acc.txt
fi

iter=1
echo "Iteration $iter: "
./train 1 model_init.txt seq_model_01.txt model_01.txt
./train 1 model_init.txt seq_model_02.txt model_02.txt
./train 1 model_init.txt seq_model_03.txt model_03.txt
./train 1 model_init.txt seq_model_04.txt model_04.txt
./train 1 model_init.txt seq_model_05.txt model_05.txt
./test modellist.txt testing_data1.txt result1.txt
./test modellist.txt testing_data2.txt result2.txt

iter=2
while [ $iter -le $finaliter ]
do

	echo "Iteration $iter: "
	./train 1 model_01.txt seq_model_01.txt model_01.txt
	./train 1 model_02.txt seq_model_02.txt model_02.txt
	./train 1 model_03.txt seq_model_03.txt model_03.txt
	./train 1 model_04.txt seq_model_04.txt model_04.txt
	./train 1 model_05.txt seq_model_05.txt model_05.txt
	./test modellist.txt testing_data1.txt result1.txt
	./test modellist.txt testing_data2.txt result2.txt
	iter=$[$iter+1]

done

sec=$SECONDS

echo "Execution time for whole script = `utility/timer.pl $sec`"
echo ""
