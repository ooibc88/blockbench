# How to run the DoNothing benchmark

To launch experiments with varying client request rates, we use 

        `python exps.py -donothing` 

All the steps are the same as for the [saturation experiment](saturation.md), except the followings:

+ Binary path, logging path settings are in `env_donothing.sh`

+ Pass the `-donothing` flag to the sript 
