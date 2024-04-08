#!/bin/sh

msg(){
	echo "hw2.sh -i INPUT -o OUTPUT [-c csv|tsv] [-j]" >&2
	echo "" >&2
	echo "Available Options:" >&2
	echo "" >&2
	echo "-i: Input file to be decoded" >&2
	echo "-o: Output directory" >&2
	echo "-c csv|tsv: Output files.[ct]sv" >&2
	echo "-j: Output info.json" >&2
	exit 1
}

json="false"

while getopts ':i:o:c:j' OPT; do
	case $OPT in
		i) input="$OPTARG";;
		o) output="$OPTARG";;
		c) file_type="$OPTARG";;
		j) json="true";;
		*) msg;;
	esac
done

# check if the input meets the format
if [ ! -e "$input" ] || [ "${input#*.hw2}" = "$input" ] || [ "$output" = "" ] || ( [ "$file_type" != "" ] && [ "$file_type" != "csv" ] && [ "$file_type" != "tsv" ] );
then
	msg
fi

# create output directory
mkdir -p "$output"

# create files.[ct]sv if needed
if [ "$file_type" = "csv" ]; then
	del=","
elif [ "$file_type" = "tsv" ]; then
	del="\t"
fi

if [ "$file_type" != "" ]; then
	echo -e "filename${del}size${del}md5${del}sha1" >> ${output}/files.${file_type}
fi

# create info.json if needed
if [ ${json} = "true" ]; then
	daytime=`date -r $(cat "$input" | jq '.date') +"%FT%T%z"`
	len=$((${#daytime}-2))
	lenp=$(($len+1))
	daytime=`echo ${daytime} | cut -c 1-$len`:`echo ${daytime} | cut -c $lenp-${#daytime}`
	cat "$input" | jq 'del(.files)'| jq --arg v "$daytime" '.date = $v' >> ${output}/info.json
fi

invalid=0
# parse the remaining information of .hw2 file
cat "$input" | jq -r '.files[] | .name + " " + .data + " " + .hash.md5 + " " + .hash."sha-1"' | {
	while read name data md5 sha; do
		name="$output"/"$name"
		folder=$(echo "$name" | awk -F"/" '{for (i=1;i<=NF-1;i++) {printf $i"/"}}')
		file=$(echo "$name" | awk -F"/" '{printf $(NF)}')
		mkdir -p "$folder"
		touch "$name"
		decode=`echo "$data" | base64 --decode`
		if [ `echo "$decode" | md5sum` != "$md5" ] || [ `echo "$decode" | sha1sum` != "$sha" ]; then
			invalid=$(( $invalid + 1))
		fi
		echo "$decode" >> "$name"
		if [ "$file_type" != "" ]; then
			echo -e "${file}${del}$((${#decode}+1))${del}${md5}${del}${sha}" >> ${output}/files.${file_type}
		fi
	done
	return ${invalid}
}
