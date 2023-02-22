sum=0 #initialize sum as 0

for i in $@; #for each input values
do
    if [ $i -gt 10 ] #TRUE if value is greater than 10
    then
    sum=$((sum+i)); #adding values which are greater than 10
    fi
done
echo $sum #total sum

exit 0