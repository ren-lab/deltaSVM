#!/usr/bin/python

#########################################
# Author: Yunjiang Qiu <serein927@gmail.com>
# File: deltasvm_pred.py
# Create Date: 2018-04-20 09:10:42
#########################################

import sys
import argparse

def main():
    parser = argparse.ArgumentParser(description='')
    parser.add_argument("-s", "--obsthreshold", dest="obs_threshold", required=True, help="obs threshold input file")
    parser.add_argument("-o", "--output", dest="out", required=True, help="output file")
    args = parser.parse_args()

    obs_thresholds = dict()
    models = dict()
    with open(args.obs_threshold) as f:
        for line in f:
            tf, threshold, model = line.rstrip().split('\t')
            obs_thresholds[tf] = float(threshold)
            models[tf] = model

    fo = open(args.out, "w")
    for tf in models:
        gkm_input = "tmp/" + tf + ".merge.gkm.tsv"
        with open(gkm_input, "r") as f:
            for line in f:
                snp, ref, alt = line.rstrip().split('\t')
                ref = float(ref)
                alt = float(alt)
                if ref >= obs_thresholds[tf] or alt >= obs_thresholds[tf]:
                    obsnp = "Y"
                else:
                    obsnp = "N"
                fo.write("{0:s}\t{1:s}\t{2:f}\t{3:f}\t{4:s}\n".format(snp, tf, ref, alt, obsnp))
    fo.close()

if __name__ == "__main__":
    sys.exit(main())

