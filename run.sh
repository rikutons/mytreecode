echo "[run.sh] -------------------------------------------------------------"
echo "[run.sh]                    Make an executable file"
echo "[run.sh] -------------------------------------------------------------"
make -C build/

echo ""
echo "[run.sh] -------------------------------------------------------------"
echo "[run.sh]                   Execute simulation program"
echo "[run.sh] -------------------------------------------------------------"
cd build
# build/a.outとするとセグフォになるのでcd挟んでます
# ./a.out input_filename 1000n_aligned.csv theta_square 0
/usr/bin/time -v ./a.out input_filename 10000n_aligned.csv count 5 <./1 > 100001s 2>100001e
/usr/bin/time -v ./a.out input_filename 10000n_aligned.csv count 5 simulator_mode 1 > 100001s 2> 100001e
/usr/bin/time -v ./a.out input_filename 10000n_aligned.csv count 5 simulator_mode 2 > 100002s 2> 100002e
/usr/bin/time -v ./a.out input_filename 10000n_aligned.csv count 5 simulator_mode 3 > 100003s 2> 100003e
/usr/bin/time -v ./a.out input_filename 100000n_aligned.csv count 5 simulator_mode 1 > 1000001s 2> 1000001e
/usr/bin/time -v ./a.out input_filename 100000n_aligned.csv count 5 simulator_mode 2 > 1000002s 2> 1000002e
/usr/bin/time -v ./a.out input_filename 100000n_aligned.csv count 5 simulator_mode 3 > 1000003s 2> 1000003e

echo ""
echo "[run.sh] -------------------------------------------------------------"
echo "[run.sh]                    Execute plot.py to plot"
echo "[run.sh] -------------------------------------------------------------"
cd ..
python3 plot.py

echo ""
echo "[run.sh] Open output/dashboard.html to see the result of simulation!"
echo "[run.sh] (Maybe chrome cannot open this html, so use firefox instead if error occurred)"
