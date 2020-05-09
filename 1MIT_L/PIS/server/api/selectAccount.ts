/// <reference path="../../commonTypes/selectAccount.d.ts" />


import { Server } from "@hapi/hapi";
import { getCredentialsAndSession } from "./auth";
import { BankAccount } from "../BankAccount";
import * as querystring from 'querystring'

function isSelectAccountRequestData(obj: any): obj is SelectAccountRequestData
{
	return typeof obj == "object" && typeof obj.accountId == "string"
}

function getAccountIdFromRequest(obj: any): string
{
	if(!isSelectAccountRequestData(obj))
	{
		throw new Error("Bad request")
	}
	return obj.accountId
}

export function AddEndpoints(server: Server)
{
	server.route([
		{
			method: "POST",
			path: "/api/selectAccount",
			handler: async (request, h) => {
				let DoErrror = (errString: string) => h.redirect(`/static/html/employee/accountSelection.html?${querystring.encode({
					error: errString
				})}`)
				let accountId: string
				try
				{
					accountId = getAccountIdFromRequest(request.payload)
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
				let account = await BankAccount.findOne({where: {identifier: accountId}})
				if(!account)
				{
					return DoErrror("Requested account does not exist")
				}
				let currentClient = session.getContent().currentClient
				if(!currentClient)
				{
					return DoErrror("Client not set in session");
				}
				if(!account.isClientAuthorized(currentClient))
				{
					return DoErrror("Client is not authorized for operations on this account")
				}
				let accNum = account.number
				await session.changeContent(content => content.selectedAccount = accNum)
				return h.redirect(`/static/html/employee/showAccount.html?${querystring.encode({
					ban: accNum
				})}`)
			}
		}
	])
}