/* passiveUDP.c - passiveUDP */

int	passivesock(const char *service, const char *transport,
		int qlen);

/*------------------------------------------------------------------------
 * passiveUDP - create a passive socket for use in a UDP server
 *------------------------------------------------------------------------
 */
int
passiveUDP(const char *service)
/*
 * Arguments:
 *      service - service associated with the desired port
 */
{
	return passivesock(service, "udp", 0);
}
