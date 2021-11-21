#include "emiter.h"

static et_dep_t EventDeps;

void ET_Init(void)
{
  EventDeps.rp = EventDeps.wp = EventDeps.size = 0;
}

el_ret_t ET_AddEventListener(et_type_t eventType, void handler())
{
  if (EventDeps.size >= MAX_ET_LISTENERS)
  {
    return EL_FULL;
  }
  et_subs_t *sub = (et_subs_t *)malloc(sizeof(et_subs_t));
  sub->eventType = eventType;
  sub->handler = handler;
  EventDeps.subs[EventDeps.wp++] = sub;
  EventDeps.wp = EventDeps.wp >= MAX_ET_LISTENERS ? 0 : EventDeps.wp;
  EventDeps.size++;
  return EL_OK;
}

el_ret_t ET_EmitEvent(et_type_t eventType, fun_params_t params[])
{
  if (EventDeps.size <= 0)
  {
    return EL_EMPTY;
  }
  for (uint8_t i = 0; i < EventDeps.size; i++)
  {
    if (EventDeps.subs[i]->eventType != eventType)
    {
      continue;
    }
    EventDeps.subs[i]->handler(params);
  }
  free(params);
  return EL_OK;
}
