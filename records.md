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
simulation: 10486.5[ms]
the number of calc: 196625975

Maximum resident set size (kbytes): 418748
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
simulation: 17527.4[ms]
write: 1381.61[ms]
read: 4127.73[ms]
the number of calc: 153537668

Maximum resident set size (kbytes): 416488
```

### hdd_async_simulator
#### m.2 ssd
```
Avarage Parameters on 5 times execution
energy error: -0.00103147[J]
simulation: 13068.9[ms]
write: 1198.21[ms]
read: 4636.39[ms]
the number of calc: 152875661

Maximum resident set size (kbytes): 416260
```

# 100000n_aligned.csv
## data
- 100000 particles  
- aligned shape(have 12500 particles on each orthant)
## records
### tree_simulator
#### m.2 ssd
```
Avarage Parameters on 1 times execution
energy error: 0.462245[J]
simulation: 143865[ms]
the number of calc: -1403729948

Maximum resident set size (kbytes): 3260588
```

### hdd_async_simulator
#### m.2 ssd
```
energy error: -0.000949004[J]
simulation: 155343[ms]
write: 11364.5[ms]
read: 43857.4[ms]
the number of calc: -1938727002

Maximum resident set size (kbytes): 3239512
```