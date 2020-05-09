/// <reference path="../../commonTypes/getClientList.d.ts" />


import { Server } from "@hapi/hapi";
import { BankClient } from "../BankClient";
import { Op } from "sequelize"

export function isGetClientListRequest(obj: any): obj is GetClientListRequest
{
	return typeof obj == "object" && (typeof obj.namePrefix == "string" || typeof obj.namePrefix == "undefined")
}

export function AddEndpoints(server: Server)
{
	server.route([
		{
			method: "POST",
			path: "/api/getClientList",
			handler: async (request, h) => {
				let payload = request.payload
				if(!isGetClientListRequest(payload))
				{
					return h.response().code(400)
				}
				let clients = await BankClient.findAll({where: {
					name: {
						[Op.startsWith]: payload.namePrefix
					}
				}})
				let response: GetClientListResponse = clients.map(client => {
					return {
						birthdate: client.bday,
						id: client.id,
						name: client.name
					}
				})
				return h.response(response)
			}
		}
	])
}