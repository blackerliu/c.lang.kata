
//#define debug_printf 	printf
#define debug_printf(x, ...) 

#define MAX_DEMOD_NUM			(1)
#define MAX_EXTRA_TUNER_NUM		(4)
#define NULL 					((void *)0)

typedef struct fe_profile {
    unsigned int demod_type;
    unsigned int demod_i2c_id;
    unsigned int demod_chip_addr;
    unsigned int tuner_type;
    unsigned int tuner_i2c_id;
    unsigned int tuner_chip_addr;
	unsigned int extra_tuner_num;
	unsigned int extra_tuner_type[MAX_EXTRA_TUNER_NUM];
	unsigned int extra_tuner_i2c_id[MAX_EXTRA_TUNER_NUM];
	unsigned int extra_tuner_chip_addr[MAX_EXTRA_TUNER_NUM];
    char extra[16];
}ClassFrontendProfile;


static ClassFrontendProfile thizFrontendProfile[MAX_DEMOD_NUM];

int profile_parse(char *profile, struct fe_profile *fe_profile, int num);
int profile_print(int fe_num);

int main(void)
{
	unsigned int fe_num = 1;
	char config_string1[]="|4:2:0x80:55:0:0x18:&0:1:7:0:0";
	char config_string2[]="|4:2:0x80:55:0:0x18:~80:0:0xc0:&0:1:7:0:0";
	char config_string3[]="|4:2:0x80:55:0:0x18:~80:0:0xc0:~55:1:0xd0:&0:1:7:0:0";
		

	printf("Test 1 ..............\n");
	printf("\n\nconfig string : %s\n", config_string1);
	memset(thizFrontendProfile, 0, (sizeof(ClassFrontendProfile)) * MAX_DEMOD_NUM);
	profile_parse(config_string1, thizFrontendProfile, fe_num);
	profile_print(fe_num);

	printf("Test 2 ..............\n");
	printf("\n\nconfig string : %s\n", config_string2);
	memset(thizFrontendProfile, 0, (sizeof(ClassFrontendProfile)) * MAX_DEMOD_NUM);
	profile_parse(config_string2, thizFrontendProfile, fe_num);
	profile_print(fe_num);

	printf("Test 3 ..............\n");
	printf("\n\nconfig string : %s\n", config_string3);
	memset(thizFrontendProfile, 0, (sizeof(ClassFrontendProfile)) * MAX_DEMOD_NUM);
	profile_parse(config_string3, thizFrontendProfile, fe_num);
	profile_print(fe_num);

}

int profile_print(int fe_num)
{
	int i = 0, j = 0;

	for(i = 0; i < fe_num; i++)
	{
		printf("---------------------------------------------------------------\n");
		printf("Demod: %d\n", i);
		printf("    -- demod type: %d\n", thizFrontendProfile[i].demod_type);
		printf("    -- demod i2c bus: %d\n", thizFrontendProfile[i].demod_i2c_id);
		printf("    -- demod i2c addr: 0x%02X\n", thizFrontendProfile[i].demod_chip_addr);
		printf("    -- tuner type: %d\n", thizFrontendProfile[i].tuner_type);
		printf("    -- tuner i2c bus: %d\n", thizFrontendProfile[i].tuner_i2c_id);
		printf("    -- tuner i2c addr: 0x%02X\n", thizFrontendProfile[i].tuner_chip_addr);
		printf("    -- extra: %s\n", thizFrontendProfile[i].extra);
		printf("    -- extra tuner num : %d\n", thizFrontendProfile[i].extra_tuner_num);

		for(j = 0; j < thizFrontendProfile[i].extra_tuner_num; j++)
		{
			printf("    ---------------------\n");
			printf("	    -- extra tuner %d\n", j);
			printf("	    -- extra tuner type: %d\n", thizFrontendProfile[i].extra_tuner_type[j]);
			printf("	    -- extra tuner i2c bus: %d\n", thizFrontendProfile[i].extra_tuner_i2c_id[j]);
			printf("	    -- extra tuner i2c addr: 0x%02X\n", thizFrontendProfile[i].extra_tuner_chip_addr[j]);
		}
	}

}
int profile_parse(char *profile, struct fe_profile *fe_profile, int num)
{
	
	int i =0, j = 0;
    char *p = profile;
    char *field =NULL;
    char *tmp;
    if (!p)
        return -1;
    for(i = 0; i < num ; i++)
    {
        if(!(field = strsep(&p,"|")))
            goto syntax_error;

        if(!(field = strsep(&p,":")))
           goto syntax_error;

        fe_profile[i].demod_type= strtoul(field, NULL, 0);
        debug_printf("fe_profile.demod_type = %d\n",fe_profile[i].demod_type);

        if(!(field = strsep(&p,":")))
            goto syntax_error;

        fe_profile[i].demod_i2c_id = strtoul(field, NULL, 0);
        debug_printf("fe_profile.demod_i2c_id = %d\n",fe_profile[i].demod_i2c_id);

        if(!(field = strsep(&p,":")))
            goto syntax_error;

        fe_profile[i].demod_chip_addr= strtoul(field, NULL, 0);
        debug_printf("fe_profile.demod_chip_addr = 0x%x\n",fe_profile[i].demod_chip_addr);

        if(!(field = strsep(&p,":")))
            goto syntax_error;

        fe_profile[i].tuner_type= strtoul(field, NULL, 0);
        debug_printf("fe_profile.tuner_type = %d\n",fe_profile[i].tuner_type);

        if(!(field = strsep(&p,":")))
            goto syntax_error;

        fe_profile[i].tuner_i2c_id= strtoul(field, NULL, 0);
        debug_printf("fe_profile.tuner_i2c_id = %d\n",fe_profile[i].tuner_i2c_id);

        if(!(field = strsep(&p,":")))
            goto syntax_error;

        fe_profile[i].tuner_chip_addr= strtoul(field, &tmp, 0);
        debug_printf("fe_profile.tuner_chip_addr = 0x%x\n",fe_profile[i].tuner_chip_addr);

		fe_profile[i].extra_tuner_num = 0;
		if( *p == '~' )
		{

        	if(!(field = strsep(&p,"~")))
            	goto syntax_error;

			j = fe_profile[i].extra_tuner_num;

        	if(!(field = strsep(&p,":")))
            	goto syntax_error;
			fe_profile[i].extra_tuner_type[j] = strtoul(field, NULL, 0);
			
			if(!(field = strsep(&p, ":")))
				goto syntax_error;
			fe_profile[i].extra_tuner_i2c_id[j] = strtoul(field, NULL, 0);
			
			if(!(field = strsep(&p, ":")))
				goto syntax_error;	
			fe_profile[i].extra_tuner_chip_addr[j] = strtoul(field, NULL, 0);

			fe_profile[i].extra_tuner_num++;
		}

		if( *p == '~' )
		{

        	if(!(field = strsep(&p,"~")))
            	goto syntax_error;

			j = fe_profile[i].extra_tuner_num;

        	if(!(field = strsep(&p,":")))
            	goto syntax_error;
			fe_profile[i].extra_tuner_type[j] = strtoul(field, NULL, 0);
			
			if(!(field = strsep(&p, ":")))
				goto syntax_error;
			fe_profile[i].extra_tuner_i2c_id[j] = strtoul(field, NULL, 0);
			
			if(!(field = strsep(&p, ":")))
				goto syntax_error;	
			fe_profile[i].extra_tuner_chip_addr[j] = strtoul(field, NULL, 0);

			fe_profile[i].extra_tuner_num++;
		}
		if( *p == '~' )
		{

        	if(!(field = strsep(&p,"~")))
            	goto syntax_error;

			j = fe_profile[i].extra_tuner_num;

        	if(!(field = strsep(&p,":")))
            	goto syntax_error;
			fe_profile[i].extra_tuner_type[j] = strtoul(field, NULL, 0);
			
			if(!(field = strsep(&p, ":")))
				goto syntax_error;
			fe_profile[i].extra_tuner_i2c_id[j] = strtoul(field, NULL, 0);
			
			if(!(field = strsep(&p, ":")))
				goto syntax_error;	
			fe_profile[i].extra_tuner_chip_addr[j] = strtoul(field, NULL, 0);

			fe_profile[i].extra_tuner_num++;
		}



        if(!(field = strsep(&p,"&")))
            goto syntax_error;

	    strncpy(fe_profile[i].extra,p,16);
        debug_printf("fe_profile.extra = %s\n",fe_profile[i].extra);
    }

    return 0;

syntax_error:
    return -1;
}
	
