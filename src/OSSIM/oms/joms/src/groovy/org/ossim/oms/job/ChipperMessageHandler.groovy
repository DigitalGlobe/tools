package org.ossim.oms.job
import groovy.transform.Synchronized

class ChipperMessageHandler implements MessageHandler
{
   ChipperMessage message
   private def chipper
   private final chipperLock = new Object()
   def execute()
   {
      if(message)
      {
         def tempJobDir = message?.jobDir as File
         def result = false
         chipper = message?.chipperParams?.newChipper()
         if(!tempJobDir.exists()) tempJobDir.mkdirs()
         if(chipper)
         {
            result = chipper.execute()
         }  
         if(result&&archiveParams&&archiveParams?.inputFile)
         {
            result = archiveParams.newArchive().execute()
         }  

         synchronized(chipperLock){
            chipper?.delete()
            chipper = null
         }     
      }
      result
   }
   @Synchronized("chipperLock")
   void abort()
   {
      chipper?.abort()
   }
}