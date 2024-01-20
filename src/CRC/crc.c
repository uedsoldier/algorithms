/**
 * @file crc.h
 * @brief Librería de implementación de funciones de verificación de redundancia cíclica (CRC o Cyclic Redundancy Check)
 * @author Ing. José Roberto Parra Trewartha
 * @version 1.0
 */

#include "crc.h"

#if defined(CRC_USE_IMPLEMENTATION_NAMES) && (CRC_USE_IMPLEMENTATION_NAMES == 1)

const char *get_crc8_implementation(uint8_t index)
{
	return crc8_implementations[index];
}

const char *get_crc16_implementation(uint8_t index)
{
	return crc16_implementations[index];
}

const char *get_crc32_implementation(uint8_t index)
{
	return crc32_implementations[index];
}

#endif

/**
 * @brief Función para obtención de polinomio generador de CRC de 8 bits.
 * @param crc_type (crc_t) dato de enumeración de tipo de CRC en cuestión
 * @return Polinomio en formato hexadecimal para cálculo de CRC de 8 bits.
 */
static uint8_t CRC8_getPoly(crc_t crc_type)
{
	uint8_t retVal;
	switch (crc_type)
	{
	case CRC8_CCITT:
	case CRC8_ITU:
	case CRC8_ROHC:
		retVal = 0x07;
		break;
	case CRC8_EBU:
	case CRC8_I_CODE:
	case CRC8_SAE_J1850:
	case CRC8_SAE_J1850_ZERO:
		retVal = 0x1D;
		break;
	case CRC8_8H2F:
		retVal = 0x2F;
		break;
	case CRC8_MAXIM:
		retVal = 0x31;
		break;
	case CRC8_DARC:
		retVal = 0x39;
		break;
	case CRC8_CDMA2000:
	case CRC8_WCDMA:
		retVal = 0x9B;
		break;
	case CRC8_DVB_S2:
		retVal = 0xD5;
		break;
	default:
		retVal = 0; // Caso sin sentido
		break;
	}
#if defined CRC_LOG && CRC_LOG == 1
	printf("\tPoly: 0x%02X\n", retVal);
#endif
	return retVal;
}

/**
 * @brief Función para obtención de polinomio generador de CRC de 16 bits.
 * @param crc_type (crc_t) dato de enumeración de tipo de CRC en cuestión
 * @return Polinomio en formato hexadecimal para cálculo de CRC de 16 bits.
 */
static uint16_t CRC16_getPoly(crc_t crc_type)
{
	uint16_t retVal;
	switch (crc_type)
	{
	case CRC16_CCITT_FALSE:
	case CRC16_XMODEM:
	case CRC16_AUG_CCITT:
	case CRC16_GENIBUS:
	case CRC16_CCITT_KERMIT:
	case CRC16_TMS37157:
	case CRC16_RIELLO:
	case CRC16_MCRF4XX:
	case CRC16_X25:
	case CRC16_A:
		retVal = 0x1021;
		break;
	case CRC16_ARC:
	case CRC16_BUYPASS:
	case CRC16_DDS110:
	case CRC16_MAXIM:
	case CRC16_USB:
	case CRC16_MODBUS:
		retVal = 0x8005;
		break;
	case CRC16_DECT_R:
	case CRC16_DECT_X:
		retVal = 0x0589;
		break;
	case CRC16_DNP:
	case CRC16_EN13757:
		retVal = 0x3D65;
		break;
	case CRC16_T10_DIF:
		retVal = 0x8BB7;
		break;
	case CRC16_TELEDISK:
		retVal = 0xA097;
		break;
	case CRC16_CDMA2000:
		retVal = 0xC867;
		break;
	default:
		retVal = 0;
		break; // Caso sin sentido
	}
#if defined CRC_LOG && CRC_LOG == 1
	printf("\tPoly: 0x%04X\n", retVal);
#endif
	return retVal;
}

/**
 * @brief Función para obtención de polinomio generador de CRC de 32 bits.
 * @param crc_type (crc_t) dato de enumeración de tipo de CRC en cuestión
 * @return Polinomio en formato hexadecimal para cálculo de CRC de 32 bits.
 */
static uint32_t CRC32_getPoly(crc_t crc_type)
{
	uint32_t retVal;
	switch (crc_type)
	{
	case CRC32_D:
		retVal = 0xA833982BUL;
		break;
	case CRC32_Q:
		retVal = 0x814141ABUL;
		break;
	case CRC32_C:
		retVal = 0x1EDC6F41UL;
		break;
	case CRC32_ISO:
	case CRC32_BZIP2:
	case CRC32_MPEG_2:
	case CRC32_POSIX:
	case CRC32_JAMCRC:
		retVal = 0x04C11DB7UL;
		break;
	case CRC32_XFER:
		retVal = 0x000000AFUL;
		break;
	// case CRC32_K1:
	// 	retVal = 0x741B8CD7UL;
	// 	break;
	// case CRC32_K2:
	// 	retVal = 0x32583499UL;
	// 	break;
	default:
		retVal = 0UL;
		break; // Caso sin sentido
	}
#if defined CRC_LOG && CRC_LOG == 1
	printf("\tPoly: 0x%08lX\n", retVal);
#endif
	return retVal;
}

/**
 * @brief Función para obtención de valores iniciales (semilla o seed) de CRC de 8 bits
 * @param crc_type (crc_t) dato de enumeración de tipo de CRC en cuestión
 * @return Valor de inicio (seed) formato hexadecimal para cálculo de CRC de 8 bits
 */
static uint8_t CRC8_getSeed(crc_t crc_type)
{
	uint8_t retVal;
	switch (crc_type)
	{
	case CRC8_CCITT:
	case CRC8_ITU:
	case CRC8_MAXIM:
	case CRC8_DARC:
	case CRC8_WCDMA:
	case CRC8_DVB_S2:
		retVal = 0;
		break;
	case CRC8_I_CODE:
		retVal = 0xFD;
		break;
	case CRC8_ROHC:
	case CRC8_EBU:
	case CRC8_CDMA2000:
	case CRC8_SAE_J1850:
	case CRC8_8H2F:
		retVal = 0xFF;
		break;
	default:
		retVal = 0;
		break; // Caso sin sentido
	}
#if defined CRC_LOG && CRC_LOG == 1
	printf("\tSeed: 0x%02X\n", retVal);
#endif
	return retVal;
}

/**
 * @brief Función para obtención de valores iniciales (semilla o seed) de CRC de 16 bits
 * @param crc_type (crc_t) dato de enumeración de tipo de CRC en cuestión
 * @return Valor de inicio (seed) formato hexadecimal para cálculo de CRC de 16 bits
 */
static uint16_t CRC16_getSeed(crc_t crc_type)
{
	uint16_t retVal;
	switch (crc_type)
	{
	case CRC16_CCITT_FALSE:
	case CRC16_GENIBUS:
	case CRC16_MCRF4XX:
	case CRC16_X25:
	case CRC16_USB:
	case CRC16_MODBUS:
	case CRC16_CDMA2000:
		retVal = 0xFFFF;
		break;
	case CRC16_XMODEM:
	case CRC16_DECT_R:
	case CRC16_DECT_X:
	case CRC16_DNP:
	case CRC16_CCITT_KERMIT:
	case CRC16_EN13757:
	case CRC16_ARC:
	case CRC16_BUYPASS:
	case CRC16_MAXIM:
	case CRC16_T10_DIF:
	case CRC16_TELEDISK:
		retVal = 0x00;
		break;
	case CRC16_AUG_CCITT:
		retVal = 0x1D0F;
		break;
	case CRC16_DDS110:
		retVal = 0x800D;
		break;
	case CRC16_TMS37157:
		retVal = 0x89EC;
		break;
	case CRC16_RIELLO:
		retVal = 0xB2AA;
		break;
	case CRC16_A:
		retVal = 0xC6C6;
		break;
	default:
		retVal = 0;
		break; // Caso sin sentido
	}
#if defined CRC_LOG && CRC_LOG == 1
	printf("\tSeed: 0x%04X\n", retVal);
#endif
	return retVal;
}

/**
 * @brief Función para obtención de valores iniciales (semilla o seed) de CRC de 32 bits
 * @param crc_type (crc_t) dato de enumeración de tipo de CRC en cuestión
 * @return Valor de inicio (seed) formato hexadecimal para cálculo de CRC de 32 bits
 */
static uint32_t CRC32_getSeed(crc_t crc_type)
{
	uint32_t retVal;
	switch (crc_type)
	{
	case CRC32_Q:
	case CRC32_POSIX:
	case CRC32_XFER:
		retVal = 0UL;
		break;
	default:
		retVal = 0xFFFFFFFFUL;
		break; // El resto de los casos
	}
#if defined CRC_LOG && CRC_LOG == 1
	printf("\tSeed: 0x%08lX\n", retVal);
#endif
	return retVal;
}

/**
 * @brief Función para verificación de valor de entrada reflejada (input reflected) de CRC para 8, 16 y 32 bits.
 * @param crc_type (crc_t) dato de enumeración de tipo de CRC en cuestión.
 * @return true si el CRC en cuestión requiere entrada de datos reflejados (invertidos), falso en caso contrario.
 */
static bool CRC_getInputReflected(crc_t crc_type)
{
	bool retVal;
	switch (crc_type)
	{
	// CRC8
	case CRC8_CCITT:
	case CRC8_ITU:
	case CRC8_I_CODE:
	case CRC8_CDMA2000:
	case CRC8_DVB_S2:
	case CRC8_SAE_J1850_ZERO:
	case CRC8_SAE_J1850:
	case CRC8_8H2F:
	// CRC16
	case CRC16_GENIBUS:
	case CRC16_XMODEM:
	case CRC16_CCITT_FALSE:
	case CRC16_AUG_CCITT:
	case CRC16_DECT_R:
	case CRC16_DECT_X:
	case CRC16_EN13757:
	case CRC16_BUYPASS:
	case CRC16_DDS110:
	case CRC16_T10_DIF:
	case CRC16_TELEDISK:
	case CRC16_CDMA2000:
	// CRC32
	case CRC32_Q:
	case CRC32_MPEG_2:
	case CRC32_XFER:
	case CRC32_BZIP2:
	case CRC32_POSIX:
		retVal = false;
		break;
	default:
		retVal = true;
		break; // Todos los casos restantes
	}
#if defined CRC_LOG && CRC_LOG == 1
	printf("\tInput reflected : %u\n", retVal);
#endif
	return retVal;
}

/**
 * @brief Función para verificación de valor de salida reflejada (output reflected) de CRC para 8, 16 y 32 bits.
 * @param crc_type (crc_t) dato de enumeración de tipo de CRC en cuestión.
 * @return true si el CRC en cuestión requiere salida de datos reflejados (invertidos), falso en caso contrario.
 */
static bool CRC_getOutputReflected(crc_t crc_type)
{
	bool retVal;
	switch (crc_type)
	{
	// CRC8
	case CRC8_CCITT:
	case CRC8_ITU:
	case CRC8_I_CODE:
	case CRC8_CDMA2000:
	case CRC8_DVB_S2:
	case CRC8_SAE_J1850_ZERO:
	case CRC8_SAE_J1850:
	case CRC8_8H2F:
	// CRC16
	case CRC16_GENIBUS:
	case CRC16_XMODEM:
	case CRC16_CCITT_FALSE:
	case CRC16_AUG_CCITT:
	case CRC16_DECT_R:
	case CRC16_DECT_X:
	case CRC16_EN13757:
	case CRC16_BUYPASS:
	case CRC16_DDS110:
	case CRC16_T10_DIF:
	case CRC16_TELEDISK:
	case CRC16_CDMA2000:
	// CRC32
	case CRC32_Q:
	case CRC32_MPEG_2:
	case CRC32_XFER:
	case CRC32_BZIP2:
	case CRC32_POSIX:
		retVal = false;
		break;
	default:
		retVal = true;
		break; // Todos los casos restantes
	}
#if defined CRC_LOG && CRC_LOG == 1
	printf("\tOutput reflected : %u\n", retVal);
#endif
	return retVal;
}

/**
 * @brief Función para obtención de valor de máscara XOR de valor de salida de CRC para 8 bits.
 * @param crc_type (crc_t) dato de enumeración de tipo de CRC en cuestión.
 * @return Máscara XOR de valor final dependiendo el CRC requerido. Si es 0, significa que el algoritmo CRC no requiere máscara XOR.
 */
static uint8_t CRC8_getFinalXOR(crc_t crc_type)
{
	uint8_t retVal;
	switch (crc_type)
	{
	case CRC8_ITU:
		retVal = 0x55;
		break;
	case CRC8_SAE_J1850:
	case CRC8_8H2F:
		retVal = 0xFF;
		break;
	default:
		retVal = 0x00;
		break; // Posibles casos restantes...
	}
#if defined CRC_LOG && CRC_LOG == 1
	printf("\tFinal XOR : 0x%02X\n", retVal);
#endif
	return retVal;
}

/**
 * @brief Función para obtención de valor de máscara XOR de valor de salida de CRC para 16 bits.
 * @param crc_type (crc_t) dato de enumeración de tipo de CRC en cuestión.
 * @return Máscara XOR de valor final dependiendo el CRC requerido. Si es 0, significa que el algoritmo CRC no requiere máscara XOR.
 */
static uint16_t CRC16_getFinalXOR(crc_t crc_type)
{
	uint16_t retVal;
	switch (crc_type)
	{
	case CRC16_XMODEM:
	case CRC16_AUG_CCITT:
	case CRC16_CCITT_FALSE:
	case CRC16_DECT_X:
	case CRC16_BUYPASS:
	case CRC16_DDS110:
	case CRC16_T10_DIF:
	case CRC16_TELEDISK:
	case CRC16_CDMA2000:
		retVal = 0x0000;
		break;
	case CRC16_GENIBUS:
	case CRC16_EN13757:
	case CRC16_X25:
	case CRC16_MAXIM:
	case CRC16_USB:
	case CRC16_DNP:
		retVal = 0xFFFF;
		break;
	case CRC16_DECT_R:
		retVal = 0x0001;
		break;
	default:
		retVal = 0;
		break; // Posibles casos restantes...
	}
#if defined CRC_LOG && CRC_LOG == 1
	printf("\tFinal XOR : 0x%04X\n", retVal);
#endif
	return retVal;
}

/**
 * @brief Función para obtención de valor de máscara XOR de valor de salida de CRC para 32 bits.
 * @param crc_type (crc_t) dato de enumeración de tipo de CRC en cuestión.
 * @return Máscara XOR de valor final dependiendo el CRC requerido. Si es 0, significa que el algoritmo CRC no requiere máscara XOR.
 */
static uint32_t CRC32_getFinalXOR(crc_t crc_type)
{
	uint32_t retVal;
	switch (crc_type)
	{
	case CRC32_BZIP2:
	case CRC32_POSIX:
	case CRC32_D:
	case CRC32_C:
	case CRC32_ISO:
		retVal = 0xFFFFFFFFUL;
		break;
	default:
		retVal = 0UL;
		break; // Posibles casos restantes...
	}
#if defined CRC_LOG && CRC_LOG == 1
	printf("\tFinal XOR : 0x%08lX\n", retVal);
#endif
	return retVal;
}

/**
 * @brief Función de implementación de algoritmos CRC para 8 bits.
 * @param data (void*) Apuntador a arreglo o estructura de datos para aplicar CRC
 * @param data_length (uint16_t) Cantidad de datos para cálculo de CRC
 * @param crc_type (crc_t) dato de enumeración de tipo de CRC en cuestión.
 * @return Valor de CRC de 8 bits para los datos introducidos.
 */
uint8_t CRC8(const void *data, uint16_t data_len, crc_t crc_type)
{
#if defined CRC_LOG && CRC_LOG == 1
	printf("\tType: %u\n", crc_type);
	printf("\tData len: %u\n", data_len);
#endif

	uint8_t crc = CRC8_getSeed(crc_type);  // Inicialización de valor "semilla"
	uint8_t poly = CRC8_getPoly(crc_type); // Obtención de polinomio

#if defined(CRC8_USE_LOOKUP_TABLE) && (CRC8_USE_LOOKUP_TABLE == 1)
	const uint8_t *p_tabla;
	switch (poly)
	{
#ifdef CRC8_0x07_LOOKUP_TABLE
	case 0x07:
		p_tabla = CRC8_0x07_table;
		break;
#endif
#ifdef CRC8_0x1D_LOOKUP_TABLE
	case 0x1D:
		p_tabla = CRC8_0x1D_table;
		break;
#endif
#ifdef CRC8_0x31_LOOKUP_TABLE
	case 0x31:
		p_tabla = CRC8_0x31_table;
		break;
#endif
#ifdef CRC8_0x39_LOOKUP_TABLE
	case 0x39:
		p_tabla = CRC8_0x39_table;
		break;
#endif
#ifdef CRC8_0x9B_LOOKUP_TABLE
	case 0x9B:
		p_tabla = CRC8_0x9B_table;
		break;
#endif
#ifdef CRC8_0xD5_LOOKUP_TABLE
	case 0xD5:
		p_tabla = CRC8_0xD5_table;
		break;
#endif
#ifdef CRC8_0x2F_LOOKUP_TABLE
	case 0x2F:
		p_tabla = CRC8_0x2F_table;
		break;
#endif
	default:
		p_tabla = CRC8_0x07_table;
		break; // Caso sin sentido
	}
#if defined CRC_LOG && CRC_LOG == 1
	printf("\tTable: ");
	for (uint16_t i = 0; i != 256; i++)
	{
		printf("0x%02X ", p_tabla[i]);
	}
	printf("\n");
#endif

#endif
	uint8_t *_buf = (uint8_t *)data; // Apuntador tipo uint8_t con dirección a los datos a verificar
	bool input_reflected = CRC_getInputReflected(crc_type);
	bool output_reflected = CRC_getOutputReflected(crc_type);
	while (data_len-- != 0)
	{
		uint8_t b = *_buf++;
		b = input_reflected ? bit_invert_Byte(b) : b;
#if !defined(CRC8_USE_LOOKUP_TABLE) || (CRC8_USE_LOOKUP_TABLE != 1)
		crc = crc ^ b;
		for (uint8_t i = 0; i != 8; i++)
			crc = (crc & 0x80) ? (uint8_t)((crc << 1) ^ poly) : (uint8_t)(crc << 1);
#else
		crc = p_tabla[b ^ crc];
		// crc = (crc << 8) ^ p_tabla[(crc >> 8) ^ b];
		// crc = (uint32_t)((crc << 8) ^ p_tabla[(uint8_t)((crc ^ ((uint32_t)(b) << 24)) >> 24)]);
#endif
	}
	crc = output_reflected ? bit_invert_Byte(crc) : crc;

	return crc ^ CRC8_getFinalXOR(crc_type);
}

/**
 * @brief Función de implementación de algoritmos CRC para 16 bits.
 * @param data (void*) Apuntador a arreglo o estructura de datos para aplicar CRC
 * @param data_length (uint16_t) Cantidad de datos para cálculo de CRC
 * @param crc_type (crc_t) dato de enumeración de tipo de CRC en cuestión.
 * @return Valor de CRC de 16 bits para los datos introducidos.
 */
uint16_t CRC16(const void *data, uint16_t data_len, crc_t crc_type)
{
#if defined CRC_LOG && CRC_LOG == 1
	printf("\tType: %u\n", crc_type);
	printf("\tData len: %u\n", data_len);
#endif
	uint16_t crc = CRC16_getSeed(crc_type);	 // Inicialización de valor "semilla"
	uint16_t poly = CRC16_getPoly(crc_type); // Obtención de polinomio

#if defined(CRC16_USE_LOOKUP_TABLE) && (CRC16_USE_LOOKUP_TABLE == 1)
	const uint16_t *p_tabla;
	switch (poly)
	{
#ifdef CRC16_0x1021_LOOKUP_TABLE
	case 0x1021:
		p_tabla = CRC16_0x1021_table;
		break;
#endif
#ifdef CRC16_0x8005_LOOKUP_TABLE
	case 0x8005:
		p_tabla = CRC16_0x8005_table;
		break;
#endif
#ifdef CRC16_0x0589_LOOKUP_TABLE
	case 0x0589:
		p_tabla = CRC16_0x0589_table;
		break;
#endif
#ifdef CRC16_0x3D65_LOOKUP_TABLE
	case 0x3D65:
		p_tabla = CRC16_0x3D65_table;
		break;
#endif
#ifdef CRC16_0x8BB7_LOOKUP_TABLE
	case 0x8BB7:
		p_tabla = CRC16_0x8BB7_table;
		break;
#endif
#ifdef CRC16_0xA097_LOOKUP_TABLE
	case 0xA097:
		p_tabla = CRC16_0xA097_table;
		break;
#endif
#ifdef CRC16_0xC867_LOOKUP_TABLE
	case 0xC867:
		p_tabla = CRC16_0xC867_table;
		break;
#endif
	default:
		p_tabla = CRC16_0x1021_table;
		break; // Caso sin sentido
	}

#if defined CRC_LOG && CRC_LOG == 1
	printf("\tTable: ");
	for (uint16_t i = 0; i != 256; i++)
	{
		printf("0x%02X ", p_tabla[i]);
	}
	printf("\n");
#endif
#endif
	uint8_t *_buf = (uint8_t *)data; // Apuntador tipo uint8_t con dirección a los datos a verificar
	bool input_reflected = CRC_getInputReflected(crc_type);
	bool output_reflected = CRC_getOutputReflected(crc_type);
	while (data_len-- != 0)
	{
		uint8_t b = *_buf++;
		b = input_reflected ? bit_invert_Byte(b) : b;
#if !defined(CRC16_USE_LOOKUP_TABLE) || (CRC16_USE_LOOKUP_TABLE != 1)
		crc = crc ^ (uint16_t)((uint16_t)b << 8);
		for (uint8_t i = 0; i != 8; i++)
			crc = (crc & 0x8000) ? (crc << 1) ^ poly : (crc << 1);
#else
		crc = (crc << 8) ^ p_tabla[(crc >> 8) ^ b];
#endif
	}
	crc = output_reflected ? bit_invert_Int16(crc) : crc;
	return crc ^ CRC16_getFinalXOR(crc_type);
}

/**
 * @brief Función de implementación de algoritmos CRC para 32 bits.
 * @param data (void*) Apuntador a arreglo o estructura de datos para aplicar CRC
 * @param data_length (uint16_t) Cantidad de datos para cálculo de CRC
 * @param crc_type (crc_t) dato de enumeración de tipo de CRC en cuestión.
 * @return Valor de CRC de 32 bits para los datos introducidos.
 */
uint32_t CRC32(const void *data, uint16_t data_len, crc_t crc_type)
{
#if defined CRC_LOG && CRC_LOG == 1
	printf("\tType: %u\n", crc_type);
	printf("\tData len: %u\n", data_len);
#endif
	uint32_t crc = CRC32_getSeed(crc_type);	 // Inicialización de valor "semilla"
	uint32_t poly = CRC32_getPoly(crc_type); // Obtención de polinomio

#if defined(CRC32_USE_LOOKUP_TABLE) && (CRC32_USE_LOOKUP_TABLE == 1)
	const uint32_t *p_tabla;

	switch (poly)
	{
#ifdef CRC32_0xA833982B_LOOKUP_TABLE
	case 0xA833982BUL:
		p_tabla = CRC32_0xA833982B_table;
		break;
#endif
#ifdef CRC32_0x814141AB_LOOKUP_TABLE
	case 0x814141ABUL:
		p_tabla = CRC32_0x814141AB_table;
		break;
#endif
#ifdef CRC32_0x1EDC6F41_LOOKUP_TABLE
	case 0x1EDC6F41UL:
		p_tabla = CRC32_0x1EDC6F41_table;
		break;
#endif
#ifdef CRC32_0x04C11DB7_LOOKUP_TABLE
	case 0x04C11DB7UL:
		p_tabla = CRC32_0x04C11DB7_table;
		break;
#endif
#ifdef CRC32_0x000000AF_LOOKUP_TABLE
	case 0x000000AFUL:
		p_tabla = CRC32_0x000000AF_table;
		break;
#endif
		// case 0x32583499UL:   p_tabla = CRC32_0x32583499_table; break;
		// case 0x741B8CD7UL:   p_tabla = CRC32_0x741B8CD7_table; break;
		// default:       p_tabla = CRC32_0xA833982B_table; break;      //Caso sin sentido
	}

#if defined CRC_LOG && CRC_LOG == 1
	printf("\tTable: ");
	for (uint16_t i = 0; i != 256; i++)
	{
		printf("0x%02X ", p_tabla[i]);
	}
	printf("\n");
#endif
#endif
	uint8_t *_buf = (uint8_t *)data; // Apuntador tipo uint8_t con dirección a los datos a verificar
	bool input_reflected = CRC_getInputReflected(crc_type);
	bool output_reflected = CRC_getOutputReflected(crc_type);
	while (data_len-- != 0)
	{
		uint8_t b = *_buf++;
		b = input_reflected ? bit_invert_Byte(b) : b;
#if !defined(CRC32_USE_LOOKUP_TABLE) || (CRC32_USE_LOOKUP_TABLE != 1)
		crc = crc ^ (((uint32_t)(b)) << 24);
		for (uint8_t i = 0; i != 8; i++)
			crc = (crc & 0x80000000UL) ? (crc << 1) ^ poly : (crc << 1);
#else
		crc = (uint32_t)((crc << 8) ^ p_tabla[(uint8_t)((crc ^ ((uint32_t)(b) << 24)) >> 24)]);
#endif
	}
	crc = output_reflected ? bit_invert_Int32(crc) : crc;

	return crc ^ CRC32_getFinalXOR(crc_type);
}

/**
 * @brief 
 * 
 * @param data 
 * @return uint8_t 
 */
static uint8_t bit_invert_Byte(uint8_t data) {
	uint8_t inverted_data = 0;
	for( uint8_t i=0 ; i != 8; i++) {
		if ( (data & (1<<i)) != 0)
			inverted_data|= (1 << (7-i));
	}
	return inverted_data;
}

/**
 * @brief 
 * 
 * @param data 
 * @return uint16_t 
 */
static uint16_t bit_invert_Int16(uint16_t data) {
    uint16_t inverted_data = 0;
    for(uint8_t i=0 ; i != 16; i++)
    {
        if ( (data & (1<<i)) != 0)
            inverted_data|= (1 << (15-i));
    }
    return inverted_data;
}

/**
 * @brief 
 * 
 * @param data 
 * @return uint32_t 
 */
static uint32_t bit_invert_Int32(uint32_t data) {
    uint32_t inverted_data = 0;
    for(uint8_t i=0 ; i != 32; i++)
    {
        if ( (data & (1UL<<i)) != 0)
            inverted_data|= (1UL << (31-i));
    }
    return inverted_data;
}