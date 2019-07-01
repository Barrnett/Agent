#ifndef AGENTMONITOR_AGENTMONITORCOMMONDEF_H_
#define AGENTMONITOR_AGENTMONITORCOMMONDEF_H_

#define AM_RETURN_ERROR_FLAG "error"
#define PE_RETURN_FAILED 0
#define PE_OK			 1

#define M_MAKERETURN_ITEM(p,s) do{	\
	strcpy((p),(s));		\
	(p)+=strlen(p);		\
	(p)[0]='\0';			\
	(p)++;				\
}while(0)

#define M_MAKERETURN_ITEM_END(p,s) do{	\
	strcpy((p),(s));			\
	(p)+=strlen(p);			\
	(p)[0]='\0';			\
	(p)[1]='\0';			\
	(p)+=2;				\
}while(0)

#define M_MAKERETURN_ITEM_S(p,s,k) do{	\
	sprintf((p),"%s=%s",(s),(k));	\
	(p)+=strlen(p);			\
	(p)[0]='\0';			\
	(p)++;				\
}while(0)

#define M_MAKERETURN_ITEM_S_END(p,s,k) do{	\
	sprintf((p),"%s=%s",(s),(k));		\
	(p)+=strlen(p);		\
	(p)[0]='\0';			\
	(p)[1]='\0';				\
	(p)+=2;						\
}while(0)

#define M_MAKERETURN_ITEM_D(p,s,k) do{	\
	sprintf((p),"%s=%d",(s),(k));		\
	(p)+=strlen(p);		\
	(p)[0]='\0';			\
	(p)++;				\
}while(0)

#define M_MAKERETURN_ITEM_D_END(p,s,k) do{	\
	sprintf((p),"%s=%d",(s),(k));		\
	(p)+=strlen(p);		\
	(p)[0]='\0';			\
	(p)[1]='\0';				\
	(p)+=2;					\
}while(0)

#define M_MAKERETURN_ITEM_U(p,s,k) do{	\
	sprintf((p),"%s=%u",(s),(k));		\
	(p)+=strlen(p);		\
	(p)[0]='\0';			\
	(p)++;				\
}while(0)

#define M_MAKERETURN_ITEM_U_END(p,s,k) do{	\
	sprintf((p),"%s=%u",(s),(k));		\
	(p)+=strlen(p);		\
	(p)[0]='\0';			\
	(p)[1]='\0';				\
	(p)+=2;				\
}while(0)

#define M_MAKERETURN_ITEM_F(p,s,k) do{	\
	sprintf((p),"%s=%0.2f",(s),(k));		\
	(p)+=strlen(p);		\
	(p)[0]='\0';			\
	(p)++;				\
}while(0)

#define M_MAKERETURN_ITEM_F_END(p,s,k) do{	\
	sprintf((p),"%s=%0.2f",(s),(k));		\
	(p)+=strlen(p);		\
	(p)[0]='\0';			\
	(p)[1]='\0';				\
	(p)+=2;					\
}while(0)

#define MX_NFLAG	1678935123
#endif //
