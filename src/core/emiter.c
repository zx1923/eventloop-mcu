#include "emiter.h"
#include "util.h"

static et_dep_t EventDeps;

el_ret_t el_addEventListener(et_type_t eventType, void handler())
{
  if (EventDeps.size >= DF_MAX_LISTENERS)
  {
    return EL_FULL;
  }
  et_subs_t *sub = (et_subs_t *)el_malloc(sizeof(et_subs_t));
  sub->eventType = eventType;
  sub->handler = handler;
  EventDeps.subs[EventDeps.wp++] = sub;
  EventDeps.wp = EventDeps.wp >= DF_MAX_LISTENERS ? 0 : EventDeps.wp;
  EventDeps.size++;
  return EL_OK;
}

el_ret_t el_emitEvent(et_type_t eventType, fun_params_t *params)
{
  if (EventDeps.size <= 0)
  {
    return EL_EMPTY;
  }
  for (uint8_t i = 0; i < EventDeps.size; i++)
  {
    if (EventDeps.subs[i]->eventType == eventType)
    {
      EventDeps.subs[i]->handler(params);
    }
  }
  return EL_OK;
}
