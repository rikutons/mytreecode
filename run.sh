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
./a.out input_filename 10000n_aligned.csv count 5

echo ""
echo "[run.sh] -------------------------------------------------------------"
echo "[run.sh]                    Execute plot.py to plot"
echo "[run.sh] -------------------------------------------------------------"
cd ..
python3 plot.py

echo ""
echo "[run.sh] Open output/dashboard.html to see the result of simulation!"
echo "[run.sh] (Maybe chrome cannot open this html, so use firefox instead if error occurred)"