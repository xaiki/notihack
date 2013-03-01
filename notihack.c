#define _GNU_SOURCE         /* See feature_test_macros(7) */

#include <dlfcn.h>
#include <stdio.h>
#include <string.h>
#include <libnotify/notify.h>
#include <libnotify/notification.h>

#define ARRAYSIZE(x) (sizeof(x)/sizeof(*x))

struct orig_fn_t
{
    NotifyNotification * (*new)    (const char *, const char *, const char *);
    gboolean             (*update) (NotifyNotification *, const char *, const char *, const char *);
} orig;

struct fake_notification
{
    const char *orig_summary;
    const char *summary;
    const char *body;
    const char *icon;
};

struct fake_notification intercept_array[] =
    {
        {
            .orig_summary = "hoho",
            .summary      = "mensaje nuestro",
            .icon         = "icono nuestro",
        },
        {
            .orig_summary = "hehe",
            .summary      = "Viva SLCFK",
        },

    };

int hack_notification (const char **summary,
                       const char **body,
                       const char **icon)
{
    int i;

    for (i = 0; i < ARRAYSIZE(intercept_array); i++)
    {
        if (! intercept_array[i].orig_summary)
            break;

        if (! strcmp (intercept_array[i].orig_summary, *summary)) {
            if (intercept_array[i].summary)
                *summary = intercept_array[i].summary;
            if (intercept_array[i].body)
                *body = intercept_array[i].body;
            if (intercept_array[i].icon)
                *icon = intercept_array[i].icon;
            return 1;
        }
    }

    return 0;
}

NotifyNotification * notify_notification_new            (const char *summary,
                                                         const char *body,
                                                         const char *icon)
{
    printf ("hacked new:\n'%s',\n'%s',\n'%s'.\n",
            summary, body, icon);
    hack_notification (&summary, &body, &icon);
    return orig.new (summary, body, icon);
}

gboolean            notify_notification_update          (NotifyNotification *notification,
                                                         const char *summary,
                                                         const char *body,
                                                         const char *icon)
{
    printf ("hacked update\n");
    return orig.update (notification, summary, body, icon);
}

void _init (void) {
    printf("Loading hack.\n");

    orig.new    = dlsym (RTLD_NEXT, "notify_notification_new");
    orig.update = dlsym (RTLD_NEXT, "notify_notification_update");

    printf ("hack table:\n");
    printf ("new\t-> %p, %p\n", orig.new, notify_notification_new);
    printf ("update\t-> %p, %p\n", orig.update, notify_notification_update);
}
