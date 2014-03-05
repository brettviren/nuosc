#!/bin/bash
nuosc -n2 -e log:-1,4,0.05 -b one:11436 -D lut:density_lut.txt -c matrix -d 0 -o vec.txt -i info.txt
diff info-ref.txt info.txt
diff vec-ref.txt vec.txt
