import { ResponseToolkit } from "@hapi/hapi";
import * as querystring from "querystring"

export function MakeErrorResponse(h: ResponseToolkit, path: string, error: string)
{
	return h.redirect(`${path}?${querystring.encode({
		error: error
	})}`)
}