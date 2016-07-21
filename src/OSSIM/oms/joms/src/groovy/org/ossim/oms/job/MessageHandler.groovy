package org.ossim.oms.job

interface MessageHandler
{
   def execute()
   void abort()
}