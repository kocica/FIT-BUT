/// <reference path="../../commonTypes/employee.d.ts" />

import { Server } from "@hapi/hapi";
import { BankAdmin } from "../BankAdmin";
import { getCredentialsAndSession } from "./auth";

export function isAddEmployeeRequest(obj: any): obj is AddEmployeeRequest
{
    return typeof obj == "object" && (typeof obj.login == "string" || typeof obj.login == "undefined")
                                  && (typeof obj.password == "string" || typeof obj.password == "undefined")
                                  && (typeof obj.name == "string" || typeof obj.name == "undefined")
                                  && (typeof obj.birthday == "string" || typeof obj.birthday == "undefined")
}

export function AddEndpoints(server: Server)
{
	server.route([
		{
			method: "POST",
			path: "/api/addEmployee",
			handler: async (request, h) => {
				let payload = request.payload

				if(!isAddEmployeeRequest(payload))
				{
					return h.response().code(400)
				}

				let [credentials, _] = await getCredentialsAndSession(request)

				if(credentials.type != "admin")
				{
					return h.response().code(403)
				}

				let admin = await credentials.getUser() as BankAdmin

				try
				{
					await admin.addEmployee(payload.login, payload.password, payload.name, payload.birthdate)
				}
				catch(e)
				{
					return h.response("Employee exists").redirect("/static/html/admin/addEmployee.html").state("employeeExists", "true", {ttl: 10000, path: "/", isSecure: false, isHttpOnly: false})
				}

				return h.redirect("/static/html/admin/addEmployee.html").state("employeeAdded", "true", {ttl: 10000, path: "/", isSecure: false, isHttpOnly: false})
			}
		}
	])
}