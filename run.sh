#!/bin/bash

#Author: Yunjiang Qiu <serein927@gmail.com>
#File: run.sh
#Create Date: 2019-03-01 16:51:31

set -e

\rm -rf data tmp out log
mkdir data tmp out log
python scripts/generate_allelic_seqs.py -f resources/hg19.fa -s input_snp.tsv -o data/selex_allelic_oligos 2>log/selex_allelic_oligos.log
scripts/deltasvm_subset_multi data/selex_allelic_oligos.ref.fa data/selex_allelic_oligos.alt.fa resources/models.weights.txt out/pbs.pred.tsv resources/threhsolds.pbs.tsv 2>log/deltasvm.log
sort -k1,1 -k3,3 -o out/pbs.pred.tsv out/pbs.pred.tsv
cat resources/threhsolds.obs.tsv | while read tf score model
do
    scripts/gkmpredict -T 1 data/selex_allelic_oligos.ref.fa gkmsvm_models/${model}.model.txt tmp/${tf}.ref.gkm.tsv &>log/${model}.ref.gkm.log
    sort -k1,1 -o tmp/${tf}.ref.gkm.tsv tmp/${tf}.ref.gkm.tsv
    scripts/gkmpredict -T 1 data/selex_allelic_oligos.alt.fa gkmsvm_models/${model}.model.txt tmp/${tf}.alt.gkm.tsv &>log/${model}.alt.gkm.log
    sort -k1,1 -o tmp/${tf}.alt.gkm.tsv tmp/${tf}.alt.gkm.tsv
    paste tmp/${tf}.ref.gkm.tsv tmp/${tf}.alt.gkm.tsv | cut -f1,2,4 > tmp/${tf}.merge.gkm.tsv
done
python scripts/obs_pred.py -s resources/threhsolds.obs.tsv -o out/obs.pred.tsv
sort -k1,1 -k2,2 -o out/obs.pred.tsv out/obs.pred.tsv
paste out/obs.pred.tsv out/pbs.pred.tsv | cut -f1-5,7,9 | sort -k7,7 -k5,5r | sed '1i snp\ttf\tallele1_bind\tallele2_bind\tseq_binding\tdeltaSVM_score\tpreferred_allele' > out/summary.pred.tsv
