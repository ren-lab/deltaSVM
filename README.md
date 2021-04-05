# deltaSVM

## Introduction
Based on HT-SELEX data from [Yin *et. al.* 2017](https://www.ncbi.nlm.nih.gov/pubmed/28473536), we trained [deltaSVM](https://www.nature.com/articles/ng.3331) models for 533 distinct transcription factors(TFs) that could predict differential TF binding to SNPs. We then evaluated the performance of 129 out of 533 models using SNP-SELEX data and selected 94 models with high accuracy (AUPRC>0.75) for genome-wide prediction.

Here, we provide scripts and resources to make predictions for differential binding to SNPs of 94 high-confidence TFs. For few SNPs of interest (<1,000), our web server [GVATdb](http://renlab.sdsc.edu/GVATdb/) is recommended. To predict a large set of SNPs, you can clone this GitHub repo and run it on a local server.

## Models
The overall model consists two parts of models. One is gkm-SVM models used to predict TF binding to sequences with `gkmpredict`, and the other is deltaSVM weights to predict allelic TF binding with `deltasvm`. We carefully evaluated all models and there are 94 models with high confidence (AUPRC > 0.75). We included all gkm-SVM models in [gkm-SVM models](gkmsvm_models/) folder and [deltaSVM models](http://renlab.sdsc.edu/yunjiang/deltaSVM/resources/models.weights.txt). 

## Dependency
[pysam](https://pysam.readthedocs.io/en/latest/api.html) is required to run the software. You can install pysam using conda or pip. `gkmpredict` is copied from [lsgkm](https://github.com/Dongwon-Lee/lsgkm) and only work for Linux. If you want to run on other platforms or have any questions about `gkmpredict`, please go to lsgkm repo for help.

## Quick Start
Here we provide a quick intro to make allelic TF binding predictions for any SNPs. The script ` run.sh` only works for 94 high-confidence models.

First, you need to obtain a list of SNPs and reference genome in fasta format (.fa file) with index(.fai file). In this repo, we used `input_snp.tsv` as the input SNPs and `hg19.fa` as the reference genome. 

Second, you need to download resources files from [here](http://renlab.sdsc.edu/yunjiang/deltaSVM), including reference genome and models, and put the downloaded `resources` folder under the current directory. Other genome builds can also be used, but you need to make sure the SNP file and fasta file are from the same genome build. Also, you need to change `hg19.fa` in the `run.sh` to corresponding files.

Then, you can run `bash run.sh` to make predictions. The final output file is `out/summary.pred.tsv`. Details for columns are listed below.

1. snp: SNP id
2. tf: TF 
3. allele1 bind/allele2 bind: Binding scores from gkm-SVM for each allele
4. seq_binding: Whether the sequence is bound by the TF 
5. deltaSVM_score: deltaSVM score for allelic TF binding  
6. preferred_allele: Preferred alleles for the TF

## Additional models for advanced users
For the potential use of other models, we include all gkm-SVM models we trained in [gkm-SVM models](http://renlab.sdsc.edu/yunjiang/deltaSVM/gkmsvm_models) and [deltaSVM models](http://renlab.sdsc.edu/yunjiang/deltaSVM/deltasvm_weights). We trained multiple models for the same TF with different SNP-SELEX experiments and parameters, so you may find multiple models for the same TF. 

But models except for 94 high-confidence models are **ONLY** for advanced users and you have to take your risk when using additional models since they are either not evaluated or failed in our benchmark. We also didn't provide any thresholds to make predictions due to the lack of data.

The script `run.sh` will not work for other models except for preconfigured 94 high-confidence models. Instead, to use other models, you need to calculate deltaSVM scores and gkm-SVM scores yourself using the command examples below.

```
model="ATF2_FL_4_KZ_TGAGGA40NTTG.subsample_20000.10mer"

\rm -rf data out log
mkdir data out log
python scripts/generate_allelic_seqs.py -f resources/hg19.fa -s input_snp.tsv -o data/selex_allelic_oligos 2>log/selex_allelic_oligos.log
perl scripts/deltasvm.pl data/selex_allelic_oligos.ref.fa data/selex_allelic_oligos.alt.fa 2>log/deltasvm.log deltaSVM_weights/${model}.weights.txt out/${model}.deltasvm_scores.tsv
scripts/gkmpredict -T 1 data/selex_allelic_oligos.ref.fa gkmsvm_models/${model}.model.txt out/${model}.ref.gkm.tsv &>log/gkmpredict.ref.log
scripts/gkmpredict -T 1 data/selex_allelic_oligos.alt.fa gkmsvm_models/${model}.model.txt out/${model}.alt.gkm.tsv &>log/gkmpredict.alt.log
```

## Citation 
If you used our models or predictions results in publication, please cite our [paper]( https://www.nature.com/articles/s41586-021-03211-0) as below.

Yan, J. et al. Systematic analysis of binding of transcription factors to noncoding variants. Nature 591, 147–151 (2021).

## Contact
If you have any questions or suggestions, please raise an issue or send us an [email](mailto:serein927@gmail.com).
 

