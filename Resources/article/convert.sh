dir="./images/temp"
for files in $(ls $dir/*.png)
do
	outname=$(basename ${files%%.png}Out.eps)
	convert $files ./images/$outname
done