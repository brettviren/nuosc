def format_dict(dat, formatter = str.format, **kwds):
    unformatted = dict(dat)
    formatted = dict()

    while unformatted:
        changed = False
        for k,v in list(unformatted.items()):
            try:
                new_v = formatter(v, **kwds)
            except KeyError:
                continue        # maybe next time
            except TypeError:   # can't be formatted
                new_v = v       # pretend we just did

            changed = True
            formatted[k] = new_v
            kwds[k] = new_v
            unformatted.pop(k)
            continue
        if not changed:
            break
        continue
    if unformatted:
        formatted.update(unformatted)
    return formatted
    
