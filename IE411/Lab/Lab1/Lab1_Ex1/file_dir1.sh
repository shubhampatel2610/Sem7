if [ -f $1 ] #check if given argument is FILE
then echo File - $1 #returns FILE NAME
elif [ -d $1 ] #check if given argument is DIRECTORY
then echo Directory - $1 #returns DIRECTORY NAME
else
echo Not found
fi