# deltaSVM

## Introduction
Based on HT-SELEX data from [Yin *et. al.*](https://www.ncbi.nlm.nih.gov/pubmed/28473536), we trained [deltaSVM](https://www.nature.com/articles/ng.3331) models for 533 distinct transcription factors(TFs) that could predict differential TF binding to SNPs. We then evaluated the performance of 129 out of 533 models using SNP-SELEX data and selected 94 models with high accuracy (AUPRC>0.75) for genome-wide prediction.

Here, we provide scripts and resources to make predictions for differential binding to SNPs of those 94 TFs. For few SNPs of interest (<1,000), our web server [GVATdb](http://renlab.sdsc.edu/GVATdb/) is recommended. To predict a large set of SNPs, you can clone this GitHub repo and run it on a local server.

## Dependency
[pysam](https://pysam.readthedocs.io/en/latest/api.html) is requried to run the software. You can install pysam using conda or pip.

## Quick Start
First, you need to obtain a list of SNPs and reference genome in fasta format(.fa file) with index(.fai file). In this repo, we used `input_snp.tsv` as the input SNPs and `hg19.fa` as the reference genome. 

Second, you need to download resources files from [Ren lab server](http://renlab.sdsc.edu/yunjiang/deltaSVM), including reference genome and models and put downloaded `resources` folder under current directory. Other genome builds can also be used, but you need to make sure the SNP file and fasta file are from the same genome build. Also, you need to change `hg19.fa` in the `run.sh` to corresponding files.

Then, you can run `bash run.sh` to make predictions. The final output file is `out/summary.pred.tsv`. Details for columns are listed below.

1. snp: SNP id
2. tf: TF 
3. allele1 bind/allele2 bind: Binding scores from gkm--SVM for each allele
4. seq_binding: Whether the sequence is bound by the TF 
5. deltaSVM_score: deltaSVM score for allelic TF binding  
6. preferred_allele: Preferred alleles for the TF 
