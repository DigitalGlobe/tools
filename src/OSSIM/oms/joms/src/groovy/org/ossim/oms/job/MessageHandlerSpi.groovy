package org.ossim.oms.job


interface MessageHandlerSpi
{
   boolean accepts(Object obj)
   MessageHandler getMessageHandlerInstance(Object obj)
}