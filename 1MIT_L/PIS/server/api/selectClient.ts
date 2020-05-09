/// <reference path="../../commonTypes/selectClient.d.ts" />


import { Server } from "@hapi/hapi";
import { getCredentialsAndSession } from "./auth";

function isSelectClientRequestData(obj: any): obj is SelectClientRequestData
{
	return typeof obj == "object" && typeof obj.clientId == "string"
}

function getClientFromRequest(obj: any): number
{
	if(!isSelectClientRequestData(obj))
	{
		throw new Error("Bad request")
	}
	return parseInt(obj.clientId)
}

export function AddEndpoints(server: Server)
{
	server.route([
		{
			method: "POST",
			path: "/api/selectClient",
			handler: async (request, h) => {
				let clientId: number
				try
				{
					clientId = getClientFromRequest(request.payload)
				}
				catch(e)
				{
					return h.response().code(400)
				}
				let [credentials, session] = await getCredentialsAndSession(request)
				if(credentials.type != "employee")
				{
					return h.response().code(403)
				}
				await session.changeContent(content => content.currentClient = clientId)
				return h.redirect("/static/html/employee/menu.html")
			}
		}
	])
}