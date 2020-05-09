/// <reference path="../../commonTypes/addAuthorizedPerson.d.ts" />

import { Server } from "@hapi/hapi";
import { BankClient } from "../BankClient";
import { BankAccount } from "../BankAccount";
import { BankEmployee } from "../BankEmployee";
import { getCredentialsAndSession } from "./auth";
import { MakeErrorResponse } from "./commonFuncs";

export function isAddAuthorizedPersonRequest(obj: any): obj is AddAuthorizedPersonRequestData
{
    return typeof obj == "object" && typeof obj.clientId == "string"
}

export function AddEndpoints(server: Server)
{
	server.route([
		{
			method: "POST",
			path: "/api/addAuthorizedPerson",
			handler: async (request, h) => {
				let payload = request.payload

				if(!isAddAuthorizedPersonRequest(payload))
				{
					return h.response().code(400)
				}
				let DoError = (err: string) => MakeErrorResponse(h, "/static/html/employee/addAuthorizedPerson.html", err)

				let [credentials, session] = await getCredentialsAndSession(request)

				if(credentials.type != "employee")
				{
					return h.response().code(403)
				}

				let accountNum = session.getContent().selectedAccount
				if(!accountNum)
				{
					return DoError("Account not set")
				}

				let employee = await credentials.getUser() as BankEmployee
				let currentClient = session.getContent().currentClient
				if(!currentClient)
				{
					return DoError("Current client not set");
				}

				let account = await BankAccount.findByPk(accountNum)
				if(!account)
				{
					return DoError("Account not found")
				}
				if(account.ownerId != currentClient)
				{
					return DoError(`Client ${currentClient} is not authorized to add authorized persons to account ${accountNum}`)
				}

				let newAuthorization  = await BankClient.findByPk(parseInt(payload.clientId))
				if(!newAuthorization)
				{
					return DoError("Person to authorize not found")
				}
				try
				{
					await employee.addAuthorizedPerson(account, newAuthorization)
				}
				catch(e)
				{
					console.log("Caught error while trying to add authorized person: "+e)
					if(e instanceof Error)
					{
						e = e.message
					}
					return DoError(e)
				}

				return h.redirect(`/static/html/employee/showAccount.html?ban=${account.number}`)
			}
		}
	])
}