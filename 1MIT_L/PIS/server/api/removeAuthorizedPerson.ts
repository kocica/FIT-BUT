/// <reference path="../../commonTypes/removeAuthorizedPerson.d.ts" />

import { Server } from "@hapi/hapi";
import { BankClient } from "../BankClient";
import { BankAccount } from "../BankAccount";
import { BankEmployee } from "../BankEmployee";
import { getCredentialsAndSession } from "./auth";
import { MakeErrorResponse } from "./commonFuncs";

export function isRemoveAuthorizedPersonRequest(obj: any): obj is RemoveAuthorizedPersonRequestData
{
    return typeof obj == "object" && typeof obj.clientId == "string"
}

export function AddEndpoints(server: Server)
{
	server.route([
		{
			method: "POST",
			path: "/api/removeAuthorizedPerson",
			handler: async (request, h) => {
				let payload = request.payload

				if(!isRemoveAuthorizedPersonRequest(payload))
				{
					return h.response().code(400)
				}
				let DoError = (err: string) => MakeErrorResponse(h, "/static/html/employee/removeAuthorizedPerson.html", err)

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
				
				let clientAuth  = await BankClient.findByPk(parseInt(payload.clientId))
 				if(!clientAuth)
 				{
 					return DoError("Authorized person not found")
 				}

				let client  = session.getContent().currentClient
				if(!client)
				{
					return DoError("Client not found")
				}
				let account = await BankAccount.findByPk(accountNum)
				if(!account)
				{
					return DoError("Account not found")
				}
				
				if(account.ownerId == clientAuth.id)
				{
					return DoError("Cannot remove authorization of the owner")
				}
				
				if(account.ownerId != client)
 				{
 					return DoError(`Client ${client} is not authorized to edit authorized persons to account ${accountNum}`)
 				}
				
				try
				{
					await employee.removeAuthorizedPerson(account, clientAuth)
				}
				catch(e)
				{
					console.log("Caught error while trying to remove authorized person: "+e)
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