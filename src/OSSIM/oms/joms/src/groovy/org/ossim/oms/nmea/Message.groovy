package org.ossim.oms.nmea

class Message
{
    def lastErrorMessage = ""
    def nmeaFields = []
    def validFirstChars=['!','$']

    void reset()
    {
        nmeaFields = []
        lastErrorMessage = ""
    }
    def computeCheckSum(def input)
    {
       def targetCheckSum = -1
       def computedCheckSum = 0
       def x = 0
       if ( input.trim() )
       {
           for ( x = 1; x < input.size(); x++ )
           {
               if ( input[x] != '*' )
               {
                   computedCheckSum ^= ((int)input[x]) % 128
               }
               else
               {
                   break
               }    
           }    
        
           if ( x != input.size() )
           {
               targetCheckSum = new BigInteger( input[x+1..x+2], 16 )
          }            
       }   
       return computedCheckSum == targetCheckSum 
    }
    def parse(String input)
    {       
        reset() 
        
        if(input.size() < 1)
        {
            lastErrorMessage = "Input is empty"
            return false;
        }
        if(!(input[0] in validFirstChars))
        {
            lastErrorMessage = "Invalid start character ${input[0]}.  Valid chars are ${validFirstChars}"
        }
        def checkSum = computeCheckSum(input)

        if ( checkSum )
        {
           nmeaFields = input.substring(1).split(',')
        }
        else
        {
           lastErrorMessage = "Checksum failed";
        }
        lastErrorMessage==""
    }
}
