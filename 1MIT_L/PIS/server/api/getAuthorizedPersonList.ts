/// <reference path="../../commonTypes/getAuthorizedPersonList.d.ts" />

import { Server } from "@hapi/hapi";
import { BankTransaction } from "../BankTransaction";
import { Op } from "sequelize"
import { BankAccount } from "../BankAccount";

export function AddEndpoints(server: Server)
{
	server.route([
		{
			method: "POST",
			path: "/api/getAuthorizedPersonList",
			handler: async (request, h) => {
                let payload = request.payload as GetAuthorizedClientListRequest
                let account = await BankAccount.findByPk(payload.accountId)
				let clients = await account!.getAuthorizedPersons()
				let response: GetAuthorizedClientListResponse = clients.map(client => {
					return {
						id:        client.id,
						name:      client.name,
						birthdate: client.bday.toDateString()
					}
				})
				return h.response(response)
			}
		}
	])
}