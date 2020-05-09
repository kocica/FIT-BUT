import { Server, ResponseToolkit, Request } from "@hapi/hapi";
import {Options as CookieOptions} from '@hapi/cookie'
import { Credentials } from "../Credentials";
import { Session } from "../Session";

export interface AuthSessionCookie
{
	sessionId: number
}

export function isAuthSessionCookie(obj: any): obj is AuthSessionCookie
{
	return obj && typeof obj === "object" && typeof obj.sessionId === "number"
}

export async function getCredentialsAndSession(req: Request): Promise<[Credentials, Session]>
{
	try
	{
		if(!req.auth.isAuthenticated)
		{
			throw new Error("request must be authenticated to use this function")
		}
		let session = req.auth.credentials
		if(!isAuthSessionCookie(session))
		{
			throw new Error("Failed to get session ID from credentials")
		}
		let dbSession = await Session.findByPk(session.sessionId)
		if(!dbSession)
		{
			//TODO: login expired page?
			throw new Error("Authenticated session cannot be found in db")
		}
		let credentials = await dbSession.getCredentials()
		return [credentials, dbSession]
	}
	catch(e)
	{
		//login screwed up, reset it
		req.cookieAuth.clear()
		throw e
	}
}

export function redirectToDefaultPage(credentials: Credentials, h: ResponseToolkit, session?: Session)
{
	if(credentials.type == "admin")
	{
		return h.redirect("/static/html/admin/index.html")
	}
	else if(credentials.type == "employee")
	{
		if(typeof session?.getContent().currentClient == "number")
		{
			return h.redirect("/static/html/employee/menu.html")
		}
		else
		{
			return h.redirect("/static/html/employee/clientSelection.html")
		}
	}
	else
	{
		throw new Error("Unknown type of credentials")
	}
}

export async function setupAuthentication(server: Server): Promise<void>
{
	//pretty much taken from https://hapi.dev/tutorials/auth/?lang=en_US
	await server.register(require("@hapi/cookie"))
	let strategyOpts: CookieOptions = {
		cookie: {
			name: "PISProj_authCookie",
			isSecure: false,
			password: "qwertyuiopasdfghjklmnbvcxzqwertyuiopasdfghjklmnbvcxz",
			isSameSite: "Lax",
			path: "/"
		},
		redirectTo: "/static/public/html/login.html",
		validateFunc: async (req, session) => {
			if(!isAuthSessionCookie(session))
			{
				return {valid: false}
			}
			let dbSession = await Session.findByPk(session.sessionId)
			if(!dbSession)
			{
				return {valid: false}
			}
			return {valid: true}
		}
	}
	server.auth.strategy("session", "cookie", strategyOpts)
	server.auth.default("session")
	server.route([
		{
			path: "/api/processLogin",
			method: "POST",
			handler: async (request, h) => {
				let payload = request.payload as {login: string, password: string}
				if(!payload.login || !payload.password)
				{
					//bad request
					return h.response("Request must contain login and password").code(400)
				}
				let credentials = await Credentials.findOne({where: {login: payload.login, password: payload.password, isTerminated: false}})
				if(!credentials)
				{
					return h.response("Login failed").redirect("/static/public/html/login.html").state("loginFailed", "true", {ttl: 10000, path: "/", isSecure: false, isHttpOnly: false})
				}
				//authentication successful
				let session = await credentials.getSession()
				if(session)
				{
					//destroy previous
					session.destroy()
				}
				session = await credentials.createSession({content: {}})
				request.cookieAuth.set({sessionId: session.id})
				return redirectToDefaultPage(credentials, h)
			},
			options: {
				auth: {
					mode: "try"
				}
			}
		},
		{
			path: "/api/logout",
			method: "POST",
			handler: (request, h) => {
				request.cookieAuth.clear()
				return h.redirect("/static/public/html/login.html")
			}
		}
	])
}
