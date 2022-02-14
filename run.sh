echo "[run.sh] -------------------------------------------------------------"
echo "[run.sh]                    Make an executable file"
echo "[run.sh] -------------------------------------------------------------"
make -C build/
echo 1 > build/tmp

echo ""
echo "[run.sh] -------------------------------------------------------------"
echo "[run.sh]                   Execute simulation program"
echo "[run.sh] -------------------------------------------------------------"
cd build
function exec() {
  echo [run.sh] run a.out $1
  echo [run] $1 >> tmp
  path=`pwd`
  sudo python3 ../bpf/bpf.py /usr/bin/time -v $path/a.out $1 >> tmp 2>> tmp
}
# build/a.outとするとセグフォになるのでcd挟んでます
# ./a.out input_filename 1000n_aligned.csv theta_square 0
# exec "input_filename 1000n_aligned.csv count 5 simulator_mode 1 t 0.05"
# exec "input_filename 1000n_aligned.csv count 5 simulator_mode 2 t 0.05"
# exec "input_filename 1000n_aligned.csv count 5 simulator_mode 3 t 0.05"
# exec "input_filename 10000n_aligned.csv count 5 simulator_mode 1 t 0.05"
# exec "input_filename 10000n_aligned.csv count 5 simulator_mode 2 t 0.05"
# exec "input_filename 10000n_aligned.csv count 5 simulator_mode 3 t 0.05"
# exec "input_filename 100000n_aligned.csv count 5 simulator_mode 1 t 0.05"
# exec "input_filename 100000n_aligned.csv count 5 simulator_mode 2 t 0.05"
# exec "input_filename 100000n_aligned.csv count 5 simulator_mode 3 t 0.05"
exec "input_filename 1000000n_aligned.csv count 5 simulator_mode 1 t 0.05"
exec "input_filename 1000000n_aligned.csv count 5 simulator_mode 2 t 0.05"
exec "input_filename 1000000n_aligned.csv count 5 simulator_mode 3 t 0.05"
grep -E '\[ms\]|Maximum|\[J\]|Avarage|number|\[run\]' tmp

echo ""
echo "[run.sh] -------------------------------------------------------------"
echo "[run.sh]                    Execute plot.py to plot"
echo "[run.sh] -------------------------------------------------------------"
cd ..
python3 plot.py

echo ""
echo "[run.sh] Open output/dashboard.html to see the result of simulation!"
echo "[run.sh] (Maybe chrome cannot open this html, so use firefox instead if error occurred)"
