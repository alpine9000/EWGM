#include "game.h"

#if TRACKLOADER==0

__EXTERNAL struct ExecBase *SysBase = 0;
__EXTERNAL struct Library *DOSBase = 0;

void
amigaos_ctor(void)
{
  if (!SysBase) {
    SysBase = *((struct ExecBase **) 4);
  }

  if (!DOSBase) {
    DOSBase = OpenLibrary("dos.library", 0);
  }
}


void
amigaos_dtor(void)
{
  if (DOSBase) {
    CloseLibrary(DOSBase);
    DOSBase = 0;
  }
}

#define NEWLIST(l) ((l)->lh_Head = (struct Node *)&(l)->lh_Tail, \
                    (l)->lh_TailPred = (struct Node *)&(l)->lh_Head)

struct MsgPort *
CreatePort(CONST_STRPTR name,LONG pri)
{
  struct MsgPort *port = 0;
  UBYTE portsig;

  if ((BYTE)(portsig=AllocSignal(-1)) >= 0) {
    if (!(port=AllocMem(sizeof(*port), MEMF_CLEAR|MEMF_PUBLIC))) {
      FreeSignal(portsig);
    } else {
      port->mp_Node.ln_Type = NT_MSGPORT;
      port->mp_Node.ln_Pri = pri;
      port->mp_Node.ln_Name = (char*)name;
      port->mp_SigBit = portsig;
      port->mp_SigTask = FindTask(NULL);
      NEWLIST(&port->mp_MsgList);
      if (port->mp_Node.ln_Name) {
        AddPort(port);
      }
    }
  }
  return port;
}


VOID
DeletePort(struct MsgPort *port)
{
  if (port->mp_Node.ln_Name) {
    RemPort(port);
  }
  FreeSignal(port->mp_SigBit);
  FreeMem(port, sizeof(*port));
}


struct IORequest *
CreateExtIO(CONST struct MsgPort *port,LONG iosize)
{
  struct IORequest *ioreq = NULL;

  if (port && (ioreq=AllocMem(iosize, MEMF_CLEAR|MEMF_PUBLIC))) {
    ioreq->io_Message.mn_Node.ln_Type = NT_REPLYMSG;
    ioreq->io_Message.mn_ReplyPort = (struct MsgPort*)port;
    ioreq->io_Message.mn_Length = iosize;
  }
  return ioreq;
}


struct IOStdReq *
CreateStdIO(CONST struct MsgPort *port)
{
  return (struct IOStdReq *)CreateExtIO(port, sizeof(struct IOStdReq));
}


VOID
DeleteExtIO(struct IORequest *ioreq)
{
  LONG i;

  i = -1;
  ioreq->io_Message.mn_Node.ln_Type = i;
  ioreq->io_Device                  = (struct Device *)i;
  ioreq->io_Unit                    = (struct Unit *)i;
  FreeMem(ioreq,ioreq->io_Message.mn_Length);
}

#else
void amigaos_ctor(void)
{

}
#endif
