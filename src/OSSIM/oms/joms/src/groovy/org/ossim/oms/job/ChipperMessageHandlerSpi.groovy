package org.ossim.oms.job

class ChipperMessageHandlerSpi implements MessageHandlerSpi
{
   boolean accepts(Object obj)
   {

      return (obj.type == "ChipperMessage")
   }

   MessageHandler getMessageHandlerInstance(Object obj)
   {
      ChipperMessageHandler result = new ChipperMessageHandler(message:new ChipperMessage())

      if(obj)
      {
         result.message.fromJson(obj)
      }

      result
   }
}