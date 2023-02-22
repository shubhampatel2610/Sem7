sum=0 #initialize sum as 0

for i in $@; #for each input values
    do
    if [ $i ]
    then
    sum=$((sum+i)); #adding all values
    fi
done
echo $sum #total sum

exit 0