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
  **HISAT2**                    ≥ 2.x                 Mapping synthetic PE reads
  **SAMtools**                  ≥ 1.10                Sorting / indexing BAM files
  **StringTie2 / StringTie3**   ≥ 2.2 / 3.x           Transcript assembly
  **Python / Bash**             ---                   Running pipeline scripts

You also need a reference genome (FASTA) and a HISAT2 index:

``` bash
hisat2-build reference-genome.fa HisatIndex/Index -p 20
```

## ⚙️ Installation

``` bash
git clone https://github.com/<your-name>/InFragLong.git
cd InFragLong
chmod +x InFragLong
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
    --fragment-length 200 \
    --read-length 76 \
    --hisat-index HisatIndex/Index \
    -p 30
```

Output example:

-   `synthetic-short.bam`

------------------------------------------------------------------------

### 2️⃣ Assembly using long + synthetic alignments (`--assembly`)

This step integrates native long-read BAM and synthetic short-read BAM
and reconstructs transcripts using StringTie.

``` bash
InFragLong --assembly \
    -l native-long.bam \
    -s synthetic-short.bam \
    -o InFragLong.gtf \
    --ass-backend stringtie2 \
    -c 2
```

Output:

-   `InFragLong.gtf` --- assembled transcripts

> **Note**\
> Minimum reads-per-base coverage defaults: - StringTie: **1**\
> - InFragLong: **2**

Increasing the threshold helps reduce false positives in noisy datasets.

------------------------------------------------------------------------

## 🔄 Optional: run StringTie manually

``` bash
stringtie native-long.bam synthetic-short.bam \
    -o manual.gtf \
    -c 2
```

## 🚨 Common issues

**HISAT2 index not detected**

``` bash
hisat2-build reference-genome.fa HisatIndex/Index
```

**Unknown backend name**

Allowed values:

    stringtie2
    stringtie3

## 📄 Citation

(Add citation when available.)

## 📬 Contact

Open an issue or pull request if you encounter problems or have
suggestions.
