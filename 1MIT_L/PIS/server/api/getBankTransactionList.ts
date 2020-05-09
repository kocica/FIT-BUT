/// <reference path="../../commonTypes/getBankTransactionList.d.ts" />

import { Server } from "@hapi/hapi";
import { BankTransaction } from "../BankTransaction";
import { Op } from "sequelize"
import { GetBankTransactionListByTypeRequest, GetBankTransactionListBySrcRequest, GetBankTransactionListByDestRequest, GetBankTransactionListResponse } from "../../commonTypes/getBankTransactionList";

export function isGetBankTransactionListByTypeRequest(obj: any): obj is GetBankTransactionListByTypeRequest
{
	return typeof obj == "object" && (typeof obj.type == "string" || typeof obj.type == "undefined")
}

export function isGetBankTransactionListBySrcRequest(obj: any): obj is GetBankTransactionListBySrcRequest
{
	return typeof obj == "object" && (typeof obj.srcAcc == "string" || typeof obj.srcAcc == "undefined")
}

export function isGetBankTransactionListByDestRequest(obj: any): obj is GetBankTransactionListByDestRequest
{
	return typeof obj == "object" && (typeof obj.destAcc == "string" || typeof obj.destAcc == "undefined")
}

export function AddEndpoints(server: Server)
{
	server.route([
		{
			method: "POST",
			path: "/api/getBankTransactionListByType",
			handler: async (request, h) => {
				let payload = request.payload
				if(!isGetBankTransactionListByTypeRequest(payload))
				{
					return h.response().code(400)
				}
				let transactions = await BankTransaction.findAll({where: {
					type: {
						[Op.eq]: payload.type
					}
				}})
				let response: GetBankTransactionListResponse = transactions.map(transaction => {
					return {
						id:                 transaction.id,
						amount:             transaction.amount,
						type:               transaction.type,
						date:               transaction.date,
						sourceAccount:      transaction.sourceAccount,
						destinationAccount: transaction.destinationAccount
					}
				})
				return h.response(response)
			}
		},
		{
			method: "POST",
			path: "/api/getBankTransactionListBySrc",
			handler: async (request, h) => {
				let payload = request.payload
				if(!isGetBankTransactionListBySrcRequest(payload))
				{
					return h.response().code(400)
				}
				let transactions = await BankTransaction.findAll({where: {
					sourceAccount: {
						[Op.eq]: payload.srcAcc
					}
				}})
				let response: GetBankTransactionListResponse = transactions.map(transaction => {
					return {
						id:                 transaction.id,
						amount:             transaction.amount,
						type:               transaction.type,
						date:               transaction.date,
						sourceAccount:      transaction.sourceAccount,
						destinationAccount: transaction.destinationAccount
					}
				})
				return h.response(response)
			}
		},
		{
			method: "POST",
			path: "/api/getBankTransactionListByDest",
			handler: async (request, h) => {
				let payload = request.payload
				if(!isGetBankTransactionListByDestRequest(payload))
				{
					return h.response().code(400)
				}
				let transactions = await BankTransaction.findAll({where: {
					destinationAccount: {
						[Op.eq]: payload.destAcc
					}
				}})
				let response: GetBankTransactionListResponse = transactions.map(transaction => {
					return {
						id:                 transaction.id,
						amount:             transaction.amount,
						type:               transaction.type,
						date:               transaction.date,
						sourceAccount:      transaction.sourceAccount,
						destinationAccount: transaction.destinationAccount
					}
				})
				return h.response(response)
			}
		}
	])
}