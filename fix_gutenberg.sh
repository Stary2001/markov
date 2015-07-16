start=`grep -n 'START OF THIS PROJECT GUTENBERG EBOOK' $1 | grep -o '^[0-9]\+:' | sed 's/:$//'`
end=`grep -n 'END OF THIS PROJECT GUTENBERG EBOOK' $1 | grep -o '^[0-9]\+:' | sed 's/:$//'`

awk "NR>$start && NR<$end" $1 > $1.fixed
mv $1 $1.orig
mv $1.fixed $1
