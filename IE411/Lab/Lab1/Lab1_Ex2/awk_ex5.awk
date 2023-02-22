BEGIN { 
    a=1;
    ORS=" " #prints output in new line
    if ((getline tmp) > 0){ #getline takes input in tmp
        space = tmp-1 #(1+(tmp-1)*2)/2; #calculate space for pyramid
        
        print "\n" #new line
        
        for(i=1;i<=tmp;i++) #for all lines
        {
            for(j=1;j<=space;j++) #prints space for each line
            {
                print " "
            }
            for(j=1;j<=a;j++) #print star for each line
            {
                print "*"
            }
            print "\n" #new line
            space--; #decrement of space
            a=a+2; #increment of star
        }
        exit 1
    }   
}
END{      
    exit 1
}
