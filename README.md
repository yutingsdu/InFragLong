# InFragLong

InFragLong is a two-stage pipeline designed to reconstruct transcripts
from long-read RNA-seq data.\
It generates synthetic paired-end (PE) reads from long reads, maps them
back to the reference genome, and then integrates both native long-read
alignments and synthetic short-read alignments for assembly.

## ✨ Key features

-   Generate synthetic paired-end reads from long reads
-   Map synthetic reads back to the genome using HISAT2
-   Combine long-read and short-read evidence for assembly
-   Support StringTie2 / StringTie3 backends
-   Modular workflow --- run each step independently

## 📦 Dependencies

Please make sure the following tools are installed and available in your
PATH:

  Tool                          Recommended version   Usage
  ----------------------------- --------------------- ------------------------------
    **HISAT2**                    ≥ 2.0.5                 Mapping synthetic PE reads
    **SAMtools**                  ≥ 1.2.0                Sorting / indexing BAM files

You also need a reference genome (FASTA) and a HISAT2 index:

``` bash
hisat2-build reference-genome.fa HisatIndex/Index -p 20
```

## ⚙️ Installation

``` bash
git clone https://github.com/yutingsdu/InFragLong.git
cd InFragLong
sh build.sh
```

Check the help:

``` bash
./InFragLong --help
```

## 🧭 Workflow overview

InFragLong consists of **two major steps**.

------------------------------------------------------------------------

### 1️⃣ Generate synthetic PE reads and map them (`--synthetic-pe`)

This step:

1.  Fragments long reads into synthetic paired-end reads\
2.  Maps the synthetic reads using HISAT2\
3.  Produces aligned BAM files for downstream assembly

``` bash
InFragLong --synthetic-pe \
    -r longreads.fastq \
    --hisat-index HisatIndex/Index \
    -p 30
```

Output example:

-   `synthetic-short.bam`

------------------------------------------------------------------------

### 2️⃣ Assembly using long + synthetic alignments (`--assembly`)

Before running this step, please make sure that the native long reads have already been aligned to the reference genome using minimap2:
``` bash
minimap2 -ax splice reference-genome.fa native-long.fastq > native-long.sam
samtools sort -o native-long.bam native-long.sam
```

This step integrates native long-read BAM and synthetic short-read BAM
and reconstructs transcripts using StringTie.

``` bash
InFragLong --assembly \
    -l native-long.bam \
    -s synthetic-short.bam \
    -o InFragLong.gtf \
    --ass-backend stringtie2 \
```

Output:

-   `infraglong.gtf` --- assembled transcripts

> **Note**\
> Minimum reads-per-base coverage defaults: 
> - StringTie: **1**
> - InFragLong: **2**

Increasing the threshold helps reduce false positives in noisy datasets.

------------------------------------------------------------------------

## 🔄 Optional: run StringTie manually

``` bash
stringtie synthetic-short.bam native-long.bam --mix -c 2 -o output.gtf 
```

## 🚨 Common issues

**HISAT2 index not detected**

``` bash
hisat2-build reference-genome.fa HisatIndex/Index
```


## 📬 Contact

yutingsdu@163.com
