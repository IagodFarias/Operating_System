import PyPDF2
import pandas as pd


# Função para extrair o texto do PDF
def extrair_texto_pdf(caminho_pdf):
    with open(caminho_pdf, 'rb') as arquivo:
        leitor = PyPDF2.PdfReader(arquivo)
        texto = ""
        for pagina in range(len(leitor.pages)):
            texto += leitor.pages[pagina].extract_text()
    return texto

# Função para organizar os dados em uma lista de dicionários
def organizar_dados(texto):
    linhas = texto.splitlines()
    dados = []
    entrada_atual = {}

    for linha in linhas:
        if any(mes in linha.lower() for mes in ["janeiro", "fevereiro", "março", "abril", "maio", "junho"]):
            continue  # Ignorar linhas de cabeçalho

        if "@" in linha:  # Detecção de e-mail para uma nova entrada
            if entrada_atual:  # Se já tiver uma entrada, armazena-a
                dados.append(entrada_atual)
            entrada_atual = {"E-mail": linha.split()[0], "Escola": "", "Data": "", "Tipo": "", "Visitantes": "", "Telefone": ""}

        if "Pública" in linha or "Privada" in linha:
            entrada_atual["Tipo"] = linha

        if any(caractere.isdigit() for caractere in linha):  # Linhas com números geralmente têm data ou telefone
            if "@" not in linha:  # Evitar linha de e-mails
                if len(linha) <= 10:  # É uma data
                    entrada_atual["Data"] = linha
                else:  # Número de telefone ou detalhes da escola
                    entrada_atual["Telefone"] = linha

        if "visitantes" in linha.lower():
            entrada_atual["Visitantes"] = linha

        if "@" not in linha and not any(caractere.isdigit() for caractere in linha) and len(linha) > 5:
            entrada_atual["Escola"] = linha

    if entrada_atual:  # Adicionar a última entrada
        dados.append(entrada_atual)
    
    return dados

# Função para salvar os dados em um arquivo Excel
def salvar_dados_excel(dados, caminho_excel):
    df = pd.DataFrame(dados)
    df.to_excel(caminho_excel, index=False)

# Caminho do PDF e do arquivo Excel de saída
caminho_pdf = 'C:/Users/engen/Desktop/seara salao.pdf'
caminho_excel = 'saida_dados.xlsx'

# Extração e organização dos dados
texto_pdf = extrair_texto_pdf(caminho_pdf)
dados_organizados = organizar_dados(texto_pdf)

# Salvando os dados em um arquivo Excel
salvar_dados_excel(dados_organizados, caminho_excel)

print(f'Dados extraídos e salvos em {caminho_excel}')
