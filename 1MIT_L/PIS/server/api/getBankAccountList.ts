/// <reference path="../../commonTypes/getBankAccountList.d.ts" />


import { Server } from "@hapi/hapi";
import { BankAccount } from "../BankAccount";
import { Op } from "sequelize"
import { getCredentialsAndSession } from "./auth";
import { BankClient } from "../BankClient";

export function isGetBankAccountListRequest(obj: any): obj is GetBankAccountListRequest
{
	return typeof obj == "object" // && (typeof obj.ownerId == "number" || typeof obj.ownerId == "undefined")
}

export function isGetBankAccountListPrefixRequest(obj: any): obj is GetBankAccountListPrefixRequest
{
	return typeof obj == "object" && (typeof obj.prefixId == "string" || typeof obj.prefixId == "undefined")
}

export function AddEndpoints(server: Server)
{
	server.route([
		{
			method: "POST",
			path: "/api/getBankAccountList",
			handler: async (request, h) => {
				let payload = request.payload
				if(!isGetBankAccountListRequest(payload))
				{
					return h.response().code(400)
				}
				let accounts = await BankAccount.findAll()
                let client = await BankClient.findByPk(payload.clientId)
				if(!client)
				{
					return h.response([])
				}
				let shouldInclude = await Promise.all(accounts.map(account => account.isClientAuthorized(client!)))
				let response: GetBankAccountListResponse = accounts.filter((_, index) => shouldInclude[index]).map(account => {
					return {
						number : account.number,
						identifier: account.identifier,
						amount : account.amount,
						ownerId : account.ownerId
					}
				})

				return h.response(response)
			}
		},
		{
			method: "POST",
			path: "/api/getBankAccountListPrefix",
			handler: async (request, h) => {
				let payload = request.payload
				if(!isGetBankAccountListPrefixRequest(payload))
				{
					return h.response().code(400)
				}
				let accounts = await BankAccount.findAll({where: {
					identifier: {
						[Op.startsWith]: payload.prefixId
					}
				}})
				let clientId = (await getCredentialsAndSession(request))[1].getContent().currentClient
				if(!clientId)
				{
					return h.response([])
				}
				let client = await BankClient.findByPk(clientId)
				if(!client)
				{
					return h.response([])
				}
				let shouldInclude = await Promise.all(accounts.map(account => account.isClientAuthorized(client!)))
				let response: GetBankAccountListResponse = accounts.filter((_, index) => shouldInclude[index]).map(account => {
					return {
						number : account.number,
						identifier: account.identifier,
						amount : account.amount,
						ownerId : account.ownerId
					}
				})

				return h.response(response)
			}
		}
	])
}
