const char g_SETTINGS_PIN[32];
const char g_SETTINGS_LOCATION[128];
const char g_SETTINGS_SOURCEKEY[128];
const char g_SETTINGS_ATTACHMENT[128];

void axia_init_globals()
{
	//! settings items
	memset(g_SETTINGS_LOCATION, 0x00, sizeof(g_SETTINGS_LOCATION));
	memset(g_SETTINGS_SOURCEKEY, 0x00, sizeof(g_SETTINGS_SOURCEKEY));
	memset(g_SETTINGS_PIN, 0x00, sizeof(g_SETTINGS_PIN));
	memset(g_SETTINGS_ATTACHMENT, 0x00, sizeof(g_SETTINGS_ATTACHMENT));
}

bool axia_process_TAP_JSONSETTINGS(HTTPResponse *response)
{
	char text[128];
	struct json_token *array = NULL;
	const struct json_token *token;
	struct json_token *settingsArray = NULL, *settings;
	bool bStatus = FALSE;

	array = parse_json2(response->buffer, response->length);
	if (array == NULL) {
		DbgPrint(eAXIA_ERROR, "Invalid TAP settings JSON!!!\n");
		return bStatus;
	}
	token = find_json_token(array, "error_code");
	if (token != NULL) {
		memset(text, 0x00, sizeof(text));
		memcpy(text, token->ptr, token->len);

		int errCode = atoi(text);
		switch (errCode) {
		case 0:
		case 2:
		{	
			bStatus = FALSE;
		} break;
		case 3:
		{
			bStatus = TRUE;	
		} break;
		case 1:
		{
			settingsArray = find_json_token(array, "settings");
			if (settingsArray != NULL) {
				if (settingsArray->type == JSON_TYPE_ARRAY && settingsArray->num_desc > 0) {
					settings = parse_json2(settingsArray->ptr+1, settingsArray->len-2);
					if (settings != NULL) {
						//! save location_name
						token = find_json_token(settings, "location_name");
						if (token) {
							memcpy(g_SETTINGS_LOCATION, token->ptr, token->len);
							g_SETTINGS_LOCATION[token->len] = '\0';
						}
						//! save source_key
						token = find_json_token(settings, "source_key");
						if (token) {
							memcpy(g_SETTINGS_SOURCEKEY, token->ptr, token->len);
							g_SETTINGS_SOURCEKEY[token->len] = '\0';
						}
						//! save pin
						token = find_json_token(settings, "pin");
						if (token) {
							memcpy(g_SETTINGS_PIN, token->ptr, token->len);
							g_SETTINGS_PIN[token->len] = '\0';
						}
						//! save attachment
						token = find_json_token(settings, "attachment");
						if (token) {
							memcpy(g_SETTINGS_ATTACHMENT, token->ptr, token->len);
							g_SETTINGS_ATTACHMENT[token->ptr] = '\0';
						}

						ufree((void *)settings);
						settings = NULL;

						bStatus = TRUE;
					}
				} else {
					DbgPring(eAXIA_ERROR, "Tag [settings] does not contains any information!\n");
					bStatus = FALSE;
				}
			} else {
				DbgPrint(eAXIA_ERROR, "Settings JSON does not contains token [settings]!\n");
				bStatus = FALSE;
			}			
		} break;
		default: bStatus = FALSE;
		}

		//! free up the memory
		ufree((void *)array);
		array = NULL;

	} else {
		DbgPring(eAXIA_ERROR, "Settings JSON does not contains token [error_code]!\n");
		bStatus = FALSE;
	}

	return bStatus;
}
