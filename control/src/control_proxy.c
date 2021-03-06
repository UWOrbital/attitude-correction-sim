#include "control_proxy.h"
#include <pthread.h>

cntrl_proxy cntrl_proxy_init(rbody_data_ref *comm_rbody, rbody_data_ref *curr_rbody, vec3 *cntrl_mom)
{
    return (cntrl_proxy){
        .comm_rbody = comm_rbody,
        .curr_rbody = curr_rbody,
        .cntrl_mom = cntrl_mom};
}

void cntrl_proxy_add_protect(cntrl_proxy *proxy, pthread_mutex_t *comm_rbody_protect, pthread_mutex_t *curr_rbody_protect, pthread_mutex_t *cntrl_mom_protect)
{
    proxy->comm_rbody_protect = comm_rbody_protect;
    proxy->curr_rbody_protect = curr_rbody_protect;
    proxy->cntrl_mom_protect = cntrl_mom_protect;
}

void cntrl_proxy_pull_comm_rbody(cntrl_proxy *proxy, rbody_data *repl)
{
    pthread_mutex_lock(proxy->comm_rbody_protect);

    if (proxy->comm_rbody->attit != NULL)
        repl->attit = *proxy->comm_rbody->attit;
    if (proxy->comm_rbody->ang_vel != NULL)
        repl->ang_vel = *proxy->comm_rbody->ang_vel;
    if (proxy->comm_rbody->iner_tensor != NULL)
        repl->iner_tensor = *proxy->comm_rbody->iner_tensor;

    pthread_mutex_unlock(proxy->comm_rbody_protect);
}

void cntrl_proxy_pull_curr_rbody(cntrl_proxy *proxy, rbody_data *repl)
{
    pthread_mutex_lock(proxy->curr_rbody_protect);

    if (proxy->curr_rbody->attit != NULL)
        repl->attit = *proxy->curr_rbody->attit;
    if (proxy->curr_rbody->ang_vel != NULL)
        repl->ang_vel = *proxy->curr_rbody->ang_vel;
    if (proxy->curr_rbody->iner_tensor != NULL)
        repl->iner_tensor = *proxy->curr_rbody->iner_tensor;

    pthread_mutex_unlock(proxy->curr_rbody_protect);
}

void cntrl_proxy_push_comm_rbody(cntrl_proxy *proxy, rbody_data *repl)
{
    pthread_mutex_lock(proxy->comm_rbody_protect);

    if (proxy->comm_rbody->attit != NULL)
        *proxy->comm_rbody->attit = repl->attit;
    if (proxy->comm_rbody->ang_vel != NULL)
        *proxy->comm_rbody->ang_vel = repl->ang_vel;
    if (proxy->comm_rbody->iner_tensor != NULL)
        *proxy->comm_rbody->iner_tensor = repl->iner_tensor;

    pthread_mutex_unlock(proxy->comm_rbody_protect);
}

void cntrl_proxy_push_cntrl_mom(cntrl_proxy *proxy, vec3 *mom)
{
    pthread_mutex_lock(proxy->cntrl_mom_protect);

    *proxy->cntrl_mom = *mom;

    pthread_mutex_unlock(proxy->cntrl_mom_protect);
}