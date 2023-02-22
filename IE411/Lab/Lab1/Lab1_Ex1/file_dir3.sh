a_dir=$(ls ./ | grep '^a') #finds the directories which name starts with 'a'
for i in $a_dir #for each directory starts with 'a'
do
    count=$(ls $i | wc -l) #counts the number of files or directories in particular directory
    if [ $count -eq 1 ] #check if count is equal to 1
    then rmdir -r $i #remove directory with count equal to 1
    fi
done