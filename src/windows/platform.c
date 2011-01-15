/*
 * Copyright (c) 2011 Mark Heily <mark@heily.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "../common/private.h"

/* FIXME: remove these as filters are implemented */
const struct filter evfilt_proc = EVFILT_NOTIMPL;
const struct filter evfilt_vnode = EVFILT_NOTIMPL;
const struct filter evfilt_signal = EVFILT_NOTIMPL;
const struct filter evfilt_write = EVFILT_NOTIMPL;
const struct filter evfilt_read = EVFILT_NOTIMPL;
const struct filter evfilt_timer = EVFILT_NOTIMPL;
const struct filter evfilt_user = EVFILT_NOTIMPL;

BOOL WINAPI DllMain(
        HINSTANCE self,
        DWORD reason,
        LPVOID unused)
{
    switch (reason) { 
        case DLL_PROCESS_ATTACH:
            /* XXX-FIXME: initialize kqtree mutex */
            if (WSAStartup(MAKEWORD(2,2), NULL) != 0)
                return (FALSE);
            /* TODO: bettererror handling */
            break;

        case DLL_PROCESS_DETACH:
            WSACleanup();
            break;
    }

    return (TRUE);
}

void
windows_kqueue_free(struct kqueue *kq)
{
/*XXX-FIXME close the _pipe() */
}

int
windows_kqueue_init(struct kqueue *kq)
{
	if (_pipe(kq->kq_sockfd, 512, _O_BINARY) == -1) {
		dbg_puts("_pipe failed");
		return (-1);
	}   
    return (0);
}

int
kevent_wait(
        struct kqueue *kq, 
        const struct timespec *timeout)
{
    return (0);
#if TODO
    int n;

    dbg_puts("waiting for events");
    kq->kq_rfds = kq->kq_fds;
    n = pselect(kq->kq_nfds, &kq->kq_rfds, NULL , NULL, timeout, NULL);
    if (n < 0) {
        if (errno == EINTR) {
            dbg_puts("signal caught");
            return (-1);
        }
        dbg_perror("pselect(2)");
        return (-1);
    }

    return (n);
#endif
}

int
kevent_copyout(struct kqueue *kq, int nready,
        struct kevent *eventlist, int nevents)
{
    return (0);
#if TODO
    struct filter *filt;
    int i, rv, nret;

    nret = 0;
    for (i = 0; (i < EVFILT_SYSCOUNT && nready > 0 && nevents > 0); i++) {
//        dbg_printf("eventlist: n = %d nevents = %d", nready, nevents);
        filt = &kq->kq_filt[i]; 
//        dbg_printf("pfd[%d] = %d", i, filt->kf_pfd);
        if (FD_ISSET(filt->kf_pfd, &kq->kq_rfds)) {
            dbg_printf("pending events for filter %d (%s)", filt->kf_id, filter_name(filt->kf_id));
            rv = filt->kf_copyout(filt, eventlist, nevents);
            if (rv < 0) {
                dbg_puts("kevent_copyout failed");
                nret = -1;
                break;
            }
            nret += rv;
            eventlist += rv;
            nevents -= rv;
            nready--;
        }
    }

    return (nret);
#endif
}
