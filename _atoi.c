int is_chain(info_t *info, char *buf, size_t *p) {
    size_t j = *p;
    if (buf[j] == '|' && buf[j + 1] == '|') {
        buf[j] = 0;
        j++;
        info->cmd_buf_type = CMD_OR;
    } else if (buf[j] == '&' && buf[j + 1] == '&') {
        buf[j] = 0;
        j++;
        info->cmd_buf_type = CMD_AND;
    } else if (buf[j] == ';') {
        buf[j] = 0;
        info->cmd_buf_type = CMD_CHAIN;
    } else {
        return 0;
    }
    *p = j;
    return 1;
}
int replace_vars(info_t *info) {
    int i;
    list_t *node;
    for (i = 1; info->argv[i]; i++) {
        if (info->argv[i][0] != '$' || !info->argv[i][1]) {
            continue;
        }
        if (!_strcmp(info->argv[i], "$?")) {
            replace_string(&(info->argv[i]), _strdup(convert_number(info->status, 10, 0)));
            continue;
        } else if (!_strcmp(info->argv[i], "$$")) {
            replace_string(&(info->argv[i]), _strdup(convert_number(getpid(), 10, 0)));
            continue;
        }
        node = node_starts_with(info->env, &(info->argv[i][1]), '=');
        if (node) {
            replace_string(&(info->argv[i]), _strdup(_strchr(node->str, '=') + 1));
        } else {
            replace_string(&(info->argv[i]), _strdup(""));
        }
    }
    return 0;
}

int replace_string(char **old, char *new) {
    if (!new) {
        return 0;
    }
    free(*old);
    *old = new;
    return 1;
}
