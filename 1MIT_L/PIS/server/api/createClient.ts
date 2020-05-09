/// <reference path="../../commonTypes/createClient.d.ts" />

import { Server } from "@hapi/hapi";
import { BankClient } from "../BankClient";
import { BankEmployee } from "../BankEmployee";
import { getCredentialsAndSession } from "./auth";
import { Credentials } from "../Credentials";

export function isCreateClientRequestData(obj: any): obj is CreateClientRequestData
{
    return typeof obj == "object" && (typeof obj.clientName == "string" || typeof obj.clientName == "undefined")
                                  && (typeof obj.clientBday == "string" || typeof obj.clientBday == "undefined")
}

export function AddEndpoints(server: Server)
{
	server.route([
		{
			method: "POST",
			path: "/api/createClient",
			handler: async (request, h) => {
                let payload = request.payload
                if(!isCreateClientRequestData(payload))
                {
                    return h.response().code(400)
                }

                // Get logged-in employee
                let [credentials, _] = await getCredentialsAndSession(request)
				if(credentials.type != "employee")
				{
					return h.response().code(403)
                }
                let employee = await credentials.getUser() as BankEmployee

                // Create new account
                try 
                {
                	let client = await employee.addClient(payload.clientName, payload.clientBday)
				}
				catch(e)
				{
					return h.response("Cannot create client").redirect("/static/html/employee/createClient.html").state("cannotCreate", "true", {ttl: 10000, path: "/", isSecure: false, isHttpOnly: false})
				}
                // Redirect to new client, but keep the old one in session
				// await session.changeContent(content => content.currentClient = client.id)
				return h.redirect("/static/html/employee/createClient.html").state("clientCreated", "true", {ttl: 10000, path: "/", isSecure: false, isHttpOnly: false})
			}
		}
	])
}