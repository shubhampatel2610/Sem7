for i in */; #check for all directories
do
    count=$(ls $i | wc -l) #counts the number of files or directories in particular directory
    if [ $count -gt 2 ] #check if number of files or directories are greater than 2
    then echo $i #returns the directory name with 2 or more number of files or directories
    fi  
done