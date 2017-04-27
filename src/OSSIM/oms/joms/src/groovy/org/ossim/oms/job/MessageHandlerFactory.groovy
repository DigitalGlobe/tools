package org.ossim.oms.job

import groovy.json.JsonSlurper
import java.util.ServiceLoader

class MessageHandlerFactory
{
   static MessageHandler getMessageHandlerInstance(String message)
   {
      MessageHandler result
      try{
         def jsonSlurper = new JsonSlurper()

         Object jsonObj = jsonSlurper.parseText(message)
         ServiceLoader<MessageHandlerSpi> loader = ServiceLoader.load(MessageHandlerSpi.class);
         Iterator<MessageHandlerSpi> it = loader.iterator();
         while (it.hasNext()&&!result) {
            MessageHandlerSpi messageHandlerSpi = it.next();

            if (messageHandlerSpi.accepts(jsonObj)) 
            {
               result =  messageHandlerSpi.getMessageHandlerInstance(jsonObj);
            }
         }
      }
      catch(e)
      {
         e.printStackTrace()
         result = null
      }

      result
   }
}