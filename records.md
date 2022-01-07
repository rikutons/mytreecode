## Definitions
- hdd_powered_simulator is commit 6d0721a2717fdd4499df152a5899338a2160b940's simulator.  
- hdd_powered_simulator 2 is commit 9ead260aab4dd97357206dacecc915b4a30c6392's simulator.  
It calculates LET as node of tree.

- m.2 ssd is ubuntu env and WD BLUE SN550 1TB ssd.
has 2400MB/s read and 1950MB/s write.
- wsl is wsl env and S120-256GB ssd.
has 550MB/s read and 510MB/s write.

# 1000n_aligned.csv
## data
- 1000 particles  
- aligned shape(have 125 particles on each orthant)
## records
### tree_simulator
#### m.2 ssd
```
Avarage Parameters on 5 times execution
energy error: -3.10838e-05[J]
simulation: 727.197[ms]
the number of calc: 11064116
```
### hdd_powered_simulator 2
#### m.2 ssd
```
Avarage Parameters on 5 times execution
energy error: -0.000816572[J]
simulation: 1536.03[ms]
write: 159.495[ms]
read: 426.081[ms]
the number of calc: 8521698
```
# 10000n_aligned.csv
## data
- 10000 particles  
- aligned shape(have 1250 particles on each orthant)
## records
### tree_simulator
#### m.2 ssd
```
Avarage Parameters on 5 times execution
energy error: -0.000147914[J]
simulation: 12354.4[ms]
the number of calc: 196625975
```
#### wsl

### hdd_powered_simulator
#### m.2 ssd
```
Initialization End: 122.06[ms]
Simulation Start
Simulation End: 20288.7[ms]
write: 2438.37[ms]
read: 4342.78[ms]
```
#### wsl
    
### hdd_powered_simulator 2
#### m.2 ssd
```
Avarage Parameters on 5 times execution
energy error: -0.00103647[J]
simulation: 20242.5[ms]
write: 2574.17[ms]
read: 4162.48[ms]
the number of calc: 153537668
```