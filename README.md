# deltaSVM

Based on HT-SELEX data from [Yin *et al*](https://www.ncbi.nlm.nih.gov/pubmed/28473536), we trained [deltaSVM](https://www.nature.com/articles/ng.3331) models for 533 distinct tanscription factors(TFs) that could predict differntial TF binding to SNPs. We then evaluated performance of 129 out of 533 models using SNP-SELEX data and selected 94 models with high accuracy (AUPRC>0.75) for genome-wide prediction.

Here we provided scripts and resources to make preditions fo those 94 TFs. For few SNPs of interest (<1,000), our web server [GVATdb](http://renlab.sdsc.edu/GVATdb/) is recommneded. To predict large set of SNPs, you can clone this GitHub repo and run it on your own server.

First, you need to obtain list of SNPs and reference genome in fasta format(.fa file) with index (.fai file). In this repo, we used input_snp.tsv as the input SNPs an hg19.fa as referenc genome. We didn't include hg19.fa and hg19.fa.fai in resources folder due to space limit but you can get it yourself from UCSC genome browser. Other genome build can also be used, but you need to make sure th SNP file and fasta file are from the same genome build.

Then, you can simply run `bash run.sh` t make predictions. The final output file is `out/summary.pred.tsv`. Details for columns are listed below.
