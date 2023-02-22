BEGIN{
    FS=" "; #field saperator in data file
}
{
        if($4<70 && $3=="Maths"){ #check if marks are less than 70 in maths
            s[$1]=$2 #stores student names in array 
        }
}
END{
    for(i in s){ #for each values in array
        print s[i] #prints the students' name
    }
}