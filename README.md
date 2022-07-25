# extref2bib
utility for extracting references from articles in biblatex format

### Description
Using an article doi this utility finds references used in the article (but not all references) and formats them into biblatex.

### Clone

```shell
git clone --recursive https://github.com/urmano/extref2bib.git
```

### Build

```shell
mkdir build/
cd build/
cmake ../extref2bib/
make
```

### Execute

```shell
./extref2bibexe $(artcile_doi)
```
